#pragma once
#include <memory>

enum class HeaderType {
    Wialon = 0x2424,
    Firmware = 0x4040
};
enum class PacketType {
    Login = 0x00,
    GPSData = 0x01,
    ACK = 0x03,
    Command = 0xFF
};
enum class ACKType {
    CustomCommand = 0x00,
    Firmware = 0x01,
    Config = 0x02
};
enum class ACKCode {
    Success = 0x00,
    ReceptionError = 0x01, // ошибка приема 
    RTLB = 0x02, // запрос на повтор отправки последнего блока
    IncorrectFile = 0x03
};
enum class RecordType : uint8_t {
    CP = 0,  // custom parameters
    PD = 1,
    IO = 2,
    LBS = 4,
    F = 5,  // Fuel
    T = 6,  // Temperature
    CAN = 7,
    C = 8,  // Counter
    AADC = 9,  // Analog ADC
    DC = 10,  // Driver Code
    EPD = 14  // Extended Position Data
};