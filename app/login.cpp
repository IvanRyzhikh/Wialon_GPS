    #include <vector>
    #include "GPSData.h"
    #include "PacketBuilder.h"
    #include "login.h"

    std::vector<unsigned char> LoginPacket::makeLoginBinary() {
        PacketBuilder builder;
        builder.Header(HeaderType::Wialon);
        builder.Type(PacketType::Login);
        builder.Seq(counter++);

        // booking a position for the length
        size_t dataStartPos = builder.packet.size();
        builder.packet.push_back(0x00);
        builder.packet.push_back(0x00);

        builder.packet.push_back(protocolVersion);
        builder.packet.push_back(flags);

        // login with 00 last byte
        for (char c : login) {
            builder.add(static_cast<unsigned char>(c));
        }
        builder.packet.push_back(0x00);
        
        // login with 00 last byte
        for (char c : password) {
            builder.add(static_cast<unsigned char>(c));
        }
        builder.packet.push_back(0x00);

        // calculating the length of the date
        size_t dataSize = static_cast<uint16_t>(builder.packet.size() - dataStartPos - 2);
        builder.packet[dataStartPos] = (dataSize >> 8) & 0xFF;
        builder.packet[dataStartPos + 1] = dataSize & 0xFF;

        builder.CRC16(GPSData::crc16_table, builder.packet);

        return builder.packet;
    }