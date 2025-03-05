#include <Arduino.h>
#include "CCSDS_header.h" // Include the CCSDS header file
#define INCLUDE_SECONDARY_HEADER

/* --- Onboard CCSDS Parser ---- */

class CCSDS_Parser
{
public:
    CCSDS_Parser() {}

    void begin()
    {
    }

    CCSDS_Packet *parseCCSDS(uint8_t *buffer)
    {
        // Just copy the primary header to get payload size
        CCSDS_PrimaryHeader primary_header;
        memcpy(&primary_header, buffer, sizeof(CCSDS_PrimaryHeader));

        // get payload size and total size
        CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + primary_header.packet_length);

        // Copy primary header
        memcpy(packet, buffer, sizeof(CCSDS_PrimaryHeader));

        // Copy payload
        memcpy(packet->data, buffer + sizeof(CCSDS_PrimaryHeader), primary_header.packet_length);

        return packet;
    }

    void printCCSDS(CCSDS_Packet *packet, bool dispPayload)
{
    // Print CCSDS Packet Details
    Serial.println("CCSDS Packet Details:");
    Serial.print("Version: ");
    Serial.println(packet->primaryHeader.version);
    
    Serial.print("Type: ");
    Serial.println(packet->primaryHeader.type);
    
    Serial.print("APID: ");
    Serial.println(packet->primaryHeader.apid);
    
    Serial.print("Sequence Flags: ");
    Serial.println(packet->primaryHeader.sequence_flags);
    
    Serial.print("Sequence Count: ");
    Serial.println(packet->primaryHeader.sequence_count);
    
    Serial.print("Packet Length: ");
    Serial.println(packet->primaryHeader.packet_length);

    if (dispPayload)
    {
        Serial.print("Payload: ");
        for (size_t i = 0; i < packet->primaryHeader.packet_length; i++)
        {
            Serial.printf("%02X ", packet->data[i]);  // Print payload in hex format
        }
        Serial.println();
    }
}


    

    void printCCSDS(const uint8_t *buffer, size_t bufferSize, bool print_data)
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
