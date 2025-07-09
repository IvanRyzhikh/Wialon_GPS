#pragma once

#include <string>
#include <memory>
#include <vector>

class LoginPacket {
private:
    std::string login;
    std::string password;
    uint8_t protocolVersion = 0x01;
    uint8_t flags = 0x44;
    uint16_t counter = 0;

public:
    LoginPacket(const std::string& login, const std::string& pwd) : login(login), password(pwd) { }

    std::vector<unsigned char> makeLoginBinary();
};