#pragma once

#include <unistd.h>
#include <string>
#include <vector>


class TcpClient {
private:
    int sockfd;
    std::string server_ip;
    int server_port;

public:
    TcpClient(const std::string& ip, int port);
    ~TcpClient();

    bool connectToServer();
    void disconnectFromServer();
    bool isConnected() const;
    bool sendLoginPacket();
    bool sendData(const std::vector<unsigned char>& data);
    bool receiveData(std::vector<unsigned char>& buffer, size_t bufferSize);
};