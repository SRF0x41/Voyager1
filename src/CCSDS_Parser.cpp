/**
 * @class CCSDS_Parser
 * @brief Class to parse and handle CCSDS packets.
 *
 * The `CCSDS_Parser` class provides methods to parse a CCSDS packet from a raw byte buffer, print the packet's details,
 * and handle the packet's primary header and payload. This class is designed to work with the `CCSDS_Packet` structure, which
 * contains a primary header, an optional secondary header, and a payload.
 *
 * ### Features:
 * - **Parsing CCSDS Packets**: The `parseCCSDS` method extracts the primary header, calculates the total packet size, and
 *   allocates memory for the packet.
 * - **Printing Packet Information**: The `printCCSDS` method allows the printing of the primary header and optionally
 *   the payload in a human-readable format. This can be useful for debugging or monitoring data flow.
 * - **Buffer-based Parsing**: The class works directly with byte buffers, which is typical in embedded systems or environments
 *   like Arduino, where data is often received as raw byte streams.
 *
 * ### Usage:
 * The `CCSDS_Parser` class can be used in embedded applications to process incoming CCSDS packets. This is commonly used in
 * space applications or telemetry/telecommand systems where CCSDS is the standard packet format.
 *
 * **Important Note**:
 * - The class currently doesn't fully implement handling of the secondary header, as indicated by the `INCLUDE_SECONDARY_HEADER`
 *   preprocessor definition.
 *
 * ### Example:
 * ```cpp
 * CCSDS_Parser parser;
 * parser.begin();
 *
 * uint8_t buffer[BUFFER_SIZE]; // Receive or create your CCSDS buffer
 * CCSDS_Packet *packet = parser.parseCCSDS(buffer);
 * parser.printCCSDS(packet, true); // Print the packet with the payload
 * ```
 */

class CCSDS_Parser
{
public:
    /**
     * @brief Default constructor for the CCSDS_Parser class.
     */
    CCSDS_Parser() {}

    /**
     * @brief Initialization method for the parser.
     * This method can be used to prepare any resources or settings for the parser.
     */
    void begin()
    {
        // Any initialization code can go here (not used currently).
    }

    /**
     * @brief Parse a raw CCSDS buffer into a CCSDS packet.
     * 
     * This method takes a raw byte buffer and extracts the primary header information. It then allocates memory
     * for the full CCSDS packet (including the payload) and returns a pointer to the `CCSDS_Packet` structure.
     * 
     * @param buffer Pointer to the raw byte buffer that contains the CCSDS packet data.
     * @return A pointer to a newly allocated `CCSDS_Packet` structure.
     */
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

    /**
     * @brief Print the details of the CCSDS packet, including primary header and optionally the payload.
     *
     * This method prints various fields of the CCSDS primary header and, if requested, the packet's payload in hexadecimal format.
     * The output is printed via `Serial.print`, which is typically used in Arduino-based applications.
     *
     * @param packet Pointer to the `CCSDS_Packet` structure to print.
     * @param dispPayload A boolean flag to indicate if the payload should be printed. If `true`, the payload will be printed; 
     *                    otherwise, only the primary header will be displayed.
     */
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

    /**
     * @brief Print the details of the CCSDS packet from a raw byte buffer.
     * 
     * This method extracts the CCSDS primary header information from the provided buffer and prints its details.
     * Optionally, it will print the packet's payload if the `print_data` flag is set to `true`.
     * 
     * @param buffer Pointer to the raw byte buffer containing the CCSDS packet.
     * @param bufferSize The size of the buffer to be parsed.
     * @param print_data A boolean flag to indicate if the payload should be printed. If `true`, the payload will be printed.
     */
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
