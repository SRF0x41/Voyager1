#include "esp_camera.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <stdio.h>
#include <stdint.h>

#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM

#include "camera_pins.h"

unsigned long lastCaptureTime = 0; // Last shooting time
int imageCount = 1;                // File Counter
bool camera_sign = false;          // Check camera status
bool sd_sign = false;              // Check sd status

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // When the serial monitor is turned on, the program starts to execute

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
    config.frame_size = FRAMESIZE_SVGA;     // 600x800 frame size
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

    Serial.println("Photos will begin in one minute, please be ready.");
}

/*Structs*/

#pragma pack(push, 1) // Ensure no padding is added to the struct

typedef struct
{
    uint8_t version;          // 3 bits version
    uint8_t type;             // 1 bit type 0 for telemetry 1 for telecommand
    uint8_t secondary_header; // 1 bit for true false
    uint8_t apid;             // [8 bits APID LSB]
    uint8_t sequence_flags;   // [2 bits Sequence Flags] | [14 bits Sequence Count MSB]
    uint8_t sequence_count;   // [8 bits Sequence Count LSB]
    uint16_t packet_length;   // [16 bits Packet Length] (Payload size - 1)
} CCSDS_PrimaryHeader;

#pragma pack(pop) // Restore default struct padding

#pragma pack(push, 1)
typedef struct
{
    uint32_t timestamp; // Example: 32-bit timestamp
    uint16_t data_id;   // Identifier for specific data type
} CCSDS_SecondaryHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    CCSDS_PrimaryHeader primaryHeader;
#ifdef INCLUDE_SECONDARY_HEADER
    CCSDS_SecondaryHeader secondaryHeader; // Optional
#endif
    uint8_t data[]; // Payload (variable length)
} CCSDS_Packet;
#pragma pack(pop)

/*Function Prototypes*/

void createPrimaryHeader_image(CCSDS_Packet *packet, size_t data_len)
{
    if (packet == nullptr)
    {
        return;
    }
    packet->primaryHeader.version = 0b000;
    packet->primaryHeader.type = 0;
    packet->primaryHeader.secondary_header = 0;
    // create a camera apid code
    packet->primaryHeader.apid = 0x7FF;
    // to bits describing firt or continuation
    packet->primaryHeader.sequence_flags = 0b10;
    // count the number of packets increments with each Packet 32 is 32nd packet
    packet->primaryHeader.sequence_count = 0;
    // set to the size of one frame
    packet->primaryHeader.packet_length = data_len;
}

void sendData(CCSDS_Packet *packet)
{
    Serial.write(packet->data, packet->primaryHeader.packet_length);
}

void loop()
{
    delay(1000); // Wait for a second before taking the next photo
    Serial.println("Capturing image...");

    // Capture a frame from the camera
    camera_fb_t *fb = esp_camera_fb_get();

    if (fb)
    {

        // Create CCSDS_Packet
        CCSDS_Packet *ccsds_packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_Packet) + fb->len);

        // Create the primary header for the packet
        createPrimaryHeader_image(ccsds_packet, fb->len);

        // Copy the image data to the CCSDS packet payload
        memcpy(ccsds_packet->data, fb->buf, fb->len);

        // Send data
        sendData(ccsds_packet);

        // Frees
        free(ccsds_packet);

        // Return the frame buffer after use
        esp_camera_fb_return(fb);
    }
    else
    {
        Serial.println("Camera capture failed");
    }
}

/*Code bibliography for setting up camera
https://github.com/limengdu/SeeedStudio-XIAO-ESP32S3-Sense-camera/blob/main/take_photos/take_photos.ino*/

/* Notes
Serial.println("Captured frame:");
        Serial.print("Height: ");
        Serial.println(fb->height);
        Serial.print("Width: ");
        Serial.println(fb->width);
        Serial.print("Length: ");
        Serial.println(fb->len);
        // Process the frame here, such as reading pixel data
        // For example, print the RGB565 pixel data
        uint16_t *pixels = (uint16_t *)fb->buf;

for (int i = 0; i < fb->width * fb->height; i++)
        {
            uint16_t pixel = pixels[i];
            uint8_t r = (pixel >> 11) & 0x1F; // Red component (5 bits)
            uint8_t g = (pixel >> 5) & 0x3F;  // Green component (6 bits)
            uint8_t b = pixel & 0x1F;         // Blue component (5 bits)

            // Print RGB values for a few pixels (e.g., first pixel)
            if (i == 0)
            {
                Serial.print("First pixel R=");
                Serial.print(r);
                Serial.print(" G=");
                Serial.print(g);
                Serial.print(" B=");
                Serial.println(b);
            }
        }*/