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
    uint16_t apid : 11;            // 11-bit APID (Application Process Identifier)
    uint8_t sequence_flags : 2;   // 2-bit Sequence Flags
    uint16_t sequence_count : 14; // 14-bit Sequence Count
    uint16_t packet_length;       // 16-bit Packet Length // why packet_length - 1?
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
