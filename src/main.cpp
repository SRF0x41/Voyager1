#include "esp_camera.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <stdio.h>
#include <stdint.h>
#include "CCSDS_header.h"
#include "CCSDS_Parser.cpp"

#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
#include "camera_pins.h"

/*** Function Prototypes ***/

unsigned long lastCaptureTime = 0; // Last shooting time
int imageCount = 1;                // File Counter
bool camera_sign = false;          // Check camera status
bool sd_sign = false;              // Check SD card status

/**
 * @brief Setup function to initialize camera and configure serial communication.
 * 
 * This function initializes the camera and sets up necessary pins and configurations.
 * It also checks if the camera initialization is successful and prepares the system
 * to start capturing images.
 */
void setup()
{
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  while (!Serial)
    ; // Wait for the serial monitor to be available

  // Set up camera pins for the Xiao Seeed Studio Sense Camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_SVGA;     // 600x800 frame size adjusted to Voyager specs close to 600x600
  config.pixel_format = PIXFORMAT_RGB565; // Uncompressed format (RGB565)
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Camera initialization
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err); // Print error if camera fails to initialize
    return;
  }

  camera_sign = true; // Set camera status to true indicating successful initialization
  Serial.println("Main Camera ready.");
}

/**
 * @brief Create a CCSDS packet that contains an image.
 * 
 * This function takes the captured image data and wraps it into a CCSDS packet
 * including the primary header and image data as the payload.
 * 
 * @param image_buffer Pointer to the image data buffer.
 * @param image_len Length of the image data.
 * @return CCSDS_Packet* Pointer to the created CCSDS packet.
 */
CCSDS_Packet *createImagePacket(uint8_t *image_buffer, size_t image_len)
{
  CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + image_len);
  if (!packet)
  {
    Serial.println("Memory allocation failed!"); // Handle memory allocation failure
    return nullptr;
  }
  
  // Initialize Primary Header with example data
  packet->primaryHeader.version = 0b001; // Version: 1
  packet->primaryHeader.type = 0;        // Type: Telemetry
  packet->primaryHeader.secondary_header = 0; // No secondary header
  packet->primaryHeader.apid = 100;      // APID: 100 (example)
  packet->primaryHeader.sequence_flags = 3;  // Sequence Flags
  packet->primaryHeader.sequence_count = 42; // Sequence Count
  packet->primaryHeader.packet_length = image_len; // Packet Length (payload size)
  
  // Copy image data into the CCSDS packet
  memcpy(packet->data, image_buffer, image_len);
  
  return packet;
}

/**
 * @brief Main loop for continuous image capturing, packet creation, and transmission.
 * 
 * This function captures an image every 5 seconds, creates a CCSDS packet containing
 * the image data, and sends it via serial. It also handles memory management for 
 * the CCSDS packet.
 */
void loop()
{
  delay(5000); // Wait for 5 seconds before taking the next photo
  // Serial.println("\nCapturing image...");

  // Capture a frame from the camera
  camera_fb_t *fb = esp_camera_fb_get(); // Capture the image as a frame buffer
  // Serial.printf("Buffer len %ld\n", fb->len);

  if (fb)
  {
    // Create the CCSDS packet for the captured image
    CCSDS_Packet *packet = createImagePacket(fb->buf, fb->len);

    size_t packet_size = sizeof(CCSDS_PrimaryHeader) + fb->len;
    // Serial.printf("Image packet len: %ld\n", packet_size);

    // Parse and print the CCSDS packet
    CCSDS_Parser parser;
    parser.printCCSDS(packet, false);

    // Serialize and send the packet
    uint8_t START_WORD[3] = {0xAA, 0xBB, 0xCC}; // Define a start word for the transmission
    Serial.write(START_WORD, sizeof(START_WORD));  // Send start word
    Serial.write(reinterpret_cast<uint8_t *>(packet), packet_size);  // Send the CCSDS packet

    // Free the allocated memory for the packet
    free(packet);

    // Return the frame buffer after use to free up memory
    esp_camera_fb_return(fb);
  }
  else
  {
    Serial.println("Camera capture failed"); // Handle case where the camera capture fails
  }
}
