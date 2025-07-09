#include "PacketBuilder.h"
#include "GPSData.h"

void PacketBuilder::Header(HeaderType header) {
    unsigned short headerValue = static_cast<unsigned short>(header);
    packet.push_back((headerValue >> 8) & 0xFF);
    packet.push_back(headerValue & 0xFF);
}
void PacketBuilder::Type(PacketType type) {
    packet.push_back(static_cast<unsigned char>(type));
}
void PacketBuilder::ACK(ACKType type) {
    packet.push_back(static_cast<unsigned char>(type));
}
void PacketBuilder::Code(ACKCode code) {
    packet.push_back(static_cast<unsigned char>(code));
}
void PacketBuilder::Seq(uint16_t seq) {
    packet.push_back((seq >> 8) & 0xFF);
    packet.push_back(seq & 0xFF);
}
void PacketBuilder::Length(size_t payloadSize) {
    uint16_t length = static_cast<uint16_t>(payloadSize);
    packet.push_back((length >> 8) & 0xFF);
    packet.push_back(length & 0xFF);
}

void PacketBuilder::addParam(uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(sensor_number);
    add(sensor_type);
    add(value);
}
    // Customer Parameters
void PacketBuilder::addCustome(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // Temperature Parameters
void PacketBuilder::addTemperature(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // Fuel Parameters
void PacketBuilder::addFuel(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // CAN Parameters
void PacketBuilder::addCAN(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // Driver Code Parameters
void PacketBuilder::addDC(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // Counter Parameters
void PacketBuilder::addCounter(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // Analog ADC Parameters
void PacketBuilder::addAADC(uint8_t count, uint8_t sensor_number, uint8_t sensor_type, uint16_t value) {
    add(count);
    addParam(sensor_number, sensor_type, value);
}
    // I/O
void PacketBuilder::setInputBit(uint8_t byteIndex, uint8_t bitIndex) {
    if (byteIndex >= 4 || bitIndex >= 8) {
        throw std::out_of_range("Invalid byte or bit index");
    }
    uint32_t mask = 1 << ((byteIndex * 8) + bitIndex);
    inputs |= mask;
}
void PacketBuilder::setOutputBit(uint8_t byteIndex, uint8_t bitIndex) {
    if (byteIndex >= 4 || bitIndex >= 8) {
        throw std::out_of_range("Invalid byte or bit index");
    }
    uint32_t mask = 1 << ((byteIndex * 8) + bitIndex);
    outputs |= mask;
}
void PacketBuilder::addIOData() {
    add(inputs);
    add(outputs);
}
    // LBS Param
void PacketBuilder::addLBS(uint8_t count, uint16_t mcc, uint16_t mnc, uint16_t lac, uint16_t cellId, int8_t rxLevel, uint8_t ta) {
    add(count);
    add(mcc);
    add(mnc);
    add(lac);
    add(cellId);
    add(rxLevel);
    add(ta);
}
    
void PacketBuilder::ProtocolVersion(uint8_t version) {
    packet.push_back(version);
}
void PacketBuilder::Flags(uint8_t flags) {
    packet.push_back(flags);
}
    
template void PacketBuilder::add<uint8_t>(uint8_t value);
template void PacketBuilder::add<uint16_t>(uint16_t value);
template void PacketBuilder::add<uint32_t>(uint32_t value);
template void PacketBuilder::add<uint64_t>(uint64_t value);