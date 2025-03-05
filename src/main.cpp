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
bool sd_sign = false;              // Check sd status

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // When the serial monitor is turned on, the program starts to execute

  // Set up camera pins for xiao seeedstudio sense camera
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
  config.pixel_format = PIXFORMAT_RGB565; // uncompressed format
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  camera_sign = true; // Camera initialization check passes

  Serial.println("Main Camera ready.");
}


CCSDS_Packet *createImagePacket(uint8_t *image_buffer, size_t image_len)
{
  CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + image_len);
  if (!packet)
  {
    Serial.println("Memory allocation failed!");
    return nullptr;
  }

  // Initialize Primary Header Example data
  packet->primaryHeader.version = 0b001;
  packet->primaryHeader.type = 0;
  packet->primaryHeader.secondary_header = 0;
  packet->primaryHeader.apid = 100;
  packet->primaryHeader.sequence_flags = 3;
  packet->primaryHeader.sequence_count = 42;
  packet->primaryHeader.packet_length = image_len;
  memcpy(packet->data, image_buffer, image_len);

  return packet;
}

void loop()
{
  delay(5000); // Wait for a second before taking the next photo
  //Serial.println("\nCapturing image...");

  // Capture a frame from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  //Serial.printf("Buffer len %ld\n", fb->len);

  if (fb)
  {
    // create image packet
    CCSDS_Packet *packet = createImagePacket(fb->buf, fb->len);

    size_t packet_size = sizeof(CCSDS_PrimaryHeader) + fb->len;
    //Serial.printf("Image packet len: %ld\n", packet_size);

    // CCSDS parsing
    CCSDS_Parser parser;
    parser.printCCSDS(packet, false);

    // Serialize and send
    Serial.write(reinterpret_cast<uint8_t *>(packet), packet_size);


    // Frees
    free(packet);

    // Return the frame buffer after use
    esp_camera_fb_return(fb);
  }
  else
  {
    Serial.println("Camera capture failed");
  }
}
