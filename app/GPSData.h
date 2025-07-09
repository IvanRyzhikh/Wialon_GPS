#pragma once

#include <memory>
#include <vector>

class GPSData {
private:

    unsigned short packetCRC = 0;
    mutable uint16_t packet_counter = 0;

public:
    uint8_t status;
    uint16_t flags;
    uint64_t latitude;
    uint64_t longitude;
    uint64_t altitude;
    uint16_t speed;
    uint16_t bearing;
    uint16_t accuracy;
    uint32_t timestamp;
    uint16_t fix_svs;
    uint8_t visible_svs;

    static const unsigned short crc16_table[256];
    static unsigned short crc16(const unsigned char* data, size_t len);
    std::vector<unsigned char> makeWialonBinary() const;
};