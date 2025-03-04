#include <iostream>
#include <cstring> // For memcpy
#include <fstream>
#include "CCSDS_header.h" // Include the CCSDS header file
#define INCLUDE_SECONDARY_HEADER

using namespace std;

class ParserCCSDS
{
private:
public:
    ParserCCSDS() {}

    void readTransmission(){
        ifstream inFile("Transmission.bin", ios::binary | ios::ate);

        if(!inFile){
            cerr << "Error opening Transmission.bin for in" << "\n";
        }
        
        // get file size
        streamsize fileSize = inFile.tellg();
        inFile.seekg(0, ios::beg); // Move back to the beggining

        // Debug print
        printf("Fize size bytes %ld\n ", fileSize);

        // Allocate buffer array
        uint8_t buffer[fileSize];

        inFile.read(reinterpret_cast<char*>(buffer), fileSize);

        inFile.close();

        printf("--- DATA LOADED FROM TRANSMISSION ---\n");
        printCCSDSPacket(buffer, fileSize, true);
        


    }

    void writeTransmission(CCSDS_Packet *packet)
    {
        size_t packetSize = sizeof(CCSDS_PrimaryHeader) + packet->primaryHeader.packet_length;

        // Serialize Packet into a Byte Array
        uint8_t buffer[packetSize];
        memcpy(buffer, packet, packetSize);

        ofstream outFile("Transmission.bin", ios::binary);

        if (!outFile)
        {
            cerr << "Error opening Tranmission.bin for out" << "\n";
        }

        outFile.write(reinterpret_cast<const char*>(buffer), packetSize);

        outFile.close();
    }

    void printCCSDSPacket(const uint8_t *buffer, size_t bufferSize, bool print_data)
    {
        if (bufferSize < sizeof(CCSDS_PrimaryHeader))
        {
            std::cerr << "Error: Buffer too small to contain a valid CCSDS packet!" << std::endl;
            return;
        }

        // Create an empty packet pointer
        CCSDS_Packet *packet = (CCSDS_Packet *)malloc(sizeof(CCSDS_PrimaryHeader) + bufferSize);

        // Read the Primary Header and copy into the empty packet
        memcpy(&packet->primaryHeader, buffer, sizeof(CCSDS_PrimaryHeader));

        // Keeping track of how many bytes have been read, the offset accounts for the primary header
        size_t memory_offset = sizeof(CCSDS_PrimaryHeader);

        // Print debug
        printf("PRIMARY BUFFER OFFSET SIZE %ld\n", memory_offset);
        printf("PRIMARY HEADER INFORMATION:\n");

        cout << "version: " << static_cast<int>(packet->primaryHeader.version) << "\n";
        cout << "type: " << static_cast<int>(packet->primaryHeader.type) << "\n";
        cout << "secondary_header: " << static_cast<int>(packet->primaryHeader.secondary_header) << "\n";
        cout << "apid: " << packet->primaryHeader.apid << "\n";
        cout << "sequence_flags: " << static_cast<int>(packet->primaryHeader.sequence_flags) << "\n";
        cout << "sequence_count: " << packet->primaryHeader.sequence_count << "\n";
        cout << "packet_length: " << packet->primaryHeader.packet_length << "\n";

        // Print for the data
        if (print_data)
        {
            size_t data_size = packet->primaryHeader.packet_length;

            // Copy buffer data into the packet data pointer
            memcpy(packet->data, buffer + memory_offset, data_size); // Copy payload data

            cout << "Payload Data: ";
            for (size_t i = 0; i < data_size; ++i)
            {
                cout << std::hex << static_cast<int>(packet->data[i]) << " ";
            }
            cout << std::dec << endl; // Return to decimal format
        }

        // Free allocated memory
        free(packet);
    }
};