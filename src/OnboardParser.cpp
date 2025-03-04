#include <Arduino.h>
#include "CCSDS_header.h" // Include the CCSDS header file
#include "FS.h"           // File System for ESP32 (SPIFFS or SD)
#include "SPIFFS.h"       // SPIFFS library for ESP32
#define INCLUDE_SECONDARY_HEADER

class ParserCCSDS
{
public:
    ParserCCSDS() {}

    void begin()
    {
    }

    void printCCSDSPacket(const uint8_t *buffer, size_t bufferSize, bool print_data)
    {
        if (bufferSize < sizeof(CCSDS_PrimaryHeader))
        {
            Serial.println("Error: Buffer too small to contain a valid CCSDS packet!");
            return;
        }

        CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + bufferSize);
        memcpy(&packet->primaryHeader, buffer, sizeof(CCSDS_PrimaryHeader));
        size_t memory_offset = sizeof(CCSDS_PrimaryHeader);

        Serial.printf("PRIMARY BUFFER OFFSET SIZE: %d\n", memory_offset);
        Serial.println("PRIMARY HEADER INFORMATION:");
        Serial.printf("Version: %d\n", packet->primaryHeader.version);
        Serial.printf("Type: %d\n", packet->primaryHeader.type);
        Serial.printf("Secondary Header: %d\n", packet->primaryHeader.secondary_header);
        Serial.printf("APID: %d\n", packet->primaryHeader.apid);
        Serial.printf("Sequence Flags: %d\n", packet->primaryHeader.sequence_flags);
        Serial.printf("Sequence Count: %d\n", packet->primaryHeader.sequence_count);
        Serial.printf("Packet Length: %d\n", packet->primaryHeader.packet_length);

        if (print_data)
        {
            size_t data_size = packet->primaryHeader.packet_length;
            memcpy(packet->data, buffer + memory_offset, data_size);

            Serial.print("Payload Data: ");
            for (size_t i = 0; i < data_size; ++i)
            {
                Serial.printf("%02X ", packet->data[i]);
            }
            Serial.println();
        }

        free(packet);
    }
};
