/**
 * @file CCSDS_header.h
 * @brief Header for the CCSDS packet structure.
 *
 * This header file defines the structure of a CCSDS (Consultative Committee for Space Data Systems) packet,
 * including the primary header, optional secondary header, and the data payload. The structure ensures proper 
 * packing for each header field and includes the necessary bitfields for each field's size.
 * 
 * The CCSDS packet format is typically used in space data communications to standardize telemetry and 
 * telecommand data. This header file includes:
 * - The `CCSDS_PrimaryHeader` structure, which contains essential metadata such as version, packet length,
 *   sequence count, and APID (Application Process Identifier).
 * - The optional `CCSDS_SecondaryHeader` structure, which may contain a timestamp and data identifier. 
 *   **Note: The `CCSDS_SecondaryHeader` structure is defined but not yet implemented. Further development is needed 
 *   to handle this secondary header properly.**
 * - The `CCSDS_Packet` structure, which integrates the primary and secondary headers with a variable-length 
 *   data payload.
 * 
 * The use of `#pragma pack` ensures that the structures are packed with no padding to match the CCSDS standard
 * for data alignment.
 */

#ifndef CCSDS_HEADER  // Consistent header guard name
#define CCSDS_HEADER

#include <cstdint>  // Use <cstdint> instead of <iostream> since no I/O is needed

#pragma pack(push, 1) // Ensure no padding

// CCSDS Primary Header
typedef struct
{
    uint8_t version : 3;          // 3-bit version
    uint8_t type : 1;             // 1-bit type (0 = telemetry, 1 = telecommand)
    uint8_t secondary_header : 1; // 1-bit secondary header flag
    uint16_t apid : 11;           // 11-bit APID (Application Process Identifier)
    uint8_t sequence_flags : 2;   // 2-bit Sequence Flags
    uint16_t sequence_count : 14; // 14-bit Sequence Count
    // once the monitor is working, cut this back to 16 and send multiple packets
    uint32_t packet_length;       // 16-bit Packet Length // why packet_length - 1?
} CCSDS_PrimaryHeader;

#pragma pack(pop) // Restore default alignment

#pragma pack(push, 1)
typedef struct
{
    uint32_t timestamp; // 32-bit timestamp
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

#endif // CCSDS_HEADER
