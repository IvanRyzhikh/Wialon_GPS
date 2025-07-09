#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include "enums.h"
#include "GPSData.h"

class PacketBuilder {
private:
    // For other params
    void addParam(uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // For I/O
    uint32_t inputs = 0;
    uint32_t outputs = 0;

public:
    std::vector<unsigned char> packet;
    void Header(HeaderType header);
    void Type(PacketType type);
    void ACK(ACKType type);
    void Code(ACKCode code);
    void Seq(uint16_t seq);
    void Length(size_t payloadSize);

    // big-endian
    template<typename T>
    void add(T value) {
        size_t size = sizeof(T);
        unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
        for (size_t i = 0; i < size; ++i) {
            packet.push_back(bytes[size - 1 - i]);
        }
    }
    // Customer Parameters
    void addCustome(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // Temperature Parameters
    void addTemperature(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // Fuel Parameters
    void addFuel(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // CAN Parameters
    void addCAN(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // Driver Code Parameters
    void addDC(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // Counter Parameters
    void addCounter(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // Analog ADC Parameters
    void addAADC(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value);
    // I/O
    void setInputBit(uint8_t byteIndex, uint8_t bitIndex);
    void setOutputBit(uint8_t byteIndex, uint8_t bitIndex);
    void addIOData();
    // LBS Param
    void addLBS(uint8_t count, uint16_t mcc, uint16_t mnc, uint16_t lac, uint16_t cellId, int8_t rxLevel, uint8_t ta);
    
    void ProtocolVersion(uint8_t version);
    void Flags(uint8_t flags);
    
    template<size_t N>
    void CRC16(const unsigned short(&crc16_table)[N], const std::vector<unsigned char>& dataToHash) {
        unsigned short checksum = GPSData::crc16(dataToHash.data(), dataToHash.size());
        packet.push_back((checksum >> 8) & 0xFF);
        packet.push_back(checksum & 0xFF);
}
};
