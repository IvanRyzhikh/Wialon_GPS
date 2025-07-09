#include "tcp.h"
#include <Logger/Logger.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "login.h"
#include "GPSData.h"

static constexpr std::string_view kModuleName = "tcp_connection";

TcpClient::TcpClient(const std::string& ip, int port) : sockfd(-1), server_ip(ip), server_port(port) {}

TcpClient::~TcpClient() {
    disconnectFromServer();
}

bool TcpClient::connectToServer() {
    struct addrinfo hints, *servinfo, *p;
    int rv;

    // Initialize the hints structure to zero
    memset(&hints, 0, sizeof(hints));

    // Set parameters for getaddrinfo:
    // AF_UNSPEC means we are willing to use both IPv4 and IPv6
    // SOCK_STREAM specifies that we are using the TCP protocol
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Retrieve a list of possible server addresses
    if ((rv = getaddrinfo(server_ip.c_str(), std::to_string(server_port).c_str(), &hints, &servinfo)) != 0) {
        LOGGER_ERROR(kModuleName, "getaddrinfo: {}", gai_strerror(rv));
        return false; // Return an error if we failed to retrieve addresses
    }

    // Iterate through all available server addresses
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // Try to create a socket for each address
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket"); // Output the socket creation error
            continue; // Move to the next address
        }

        // Try to connect to the server
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd); // Close the socket if connection fails
            perror("client: connect"); // Output the connection error
            continue; // Move to the next address
        }

        // If the connection is successful, exit the loop
        break;
    }

    // If none of the addresses worked
    if (p == NULL) {
        LOGGER_ERROR(kModuleName, "client: failed to connect");
        freeaddrinfo(servinfo); // Free the resources
        return false; // Return an error
    }

    freeaddrinfo(servinfo); // Free the resources after use

    // Log the successful connection
    LOGGER_INFO(kModuleName, "Connected to server at {}:{}, sock_fd={}", server_ip, server_port, sockfd);
    return true;
}

void TcpClient::disconnectFromServer() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
        LOGGER_INFO(kModuleName, "Disconnected from server");
    }
}

bool TcpClient::isConnected() const {
    return sockfd != -1;
}

bool TcpClient::sendLoginPacket() {
    if (!isConnected()) {
        if (!connectToServer()) {
            LOGGER_ERROR(kModuleName, "Failed to reconnect to the server.");
            return false;
        }
    }

    LoginPacket login_packet("862175060075212", "");
    std::vector<unsigned char> packet = login_packet.makeLoginBinary();

    ssize_t bytes_sent = write(sockfd, packet.data(), packet.size());
    if (bytes_sent <= 0) {
        LOGGER_ERROR(kModuleName, "Failed to send login packet: {}", strerror(errno));
        disconnectFromServer();
        return false;
    }

    LOGGER_INFO(kModuleName, "Login packet sent successfully.");
    return true;
}

bool TcpClient::sendData(const std::vector<unsigned char>& data) {
    if (!isConnected()) {
        if (!connectToServer()) {
            LOGGER_ERROR(kModuleName, "Failed to reconnect to the server.");
            return false;
        }
    }

    ssize_t bytes_sent = write(sockfd, data.data(), data.size());
    if (bytes_sent <= 0) {
        LOGGER_ERROR(kModuleName, "Failed to send data: {}", strerror(errno));
        disconnectFromServer();
        return false;
    }

    LOGGER_INFO(kModuleName, "Data packet sent successfully ({} bytes).", bytes_sent);
    return true;
}

/*
    Receive data from the server
    Read the specified number of bytes from the socket and saves them to the buffer
*/
bool TcpClient::receiveData(std::vector<unsigned char>& buffer, size_t bufferSize) {
    if (!isConnected()) {
        LOGGER_ERROR(kModuleName, "Not connected to the server.");
        return false;
    }

    buffer.resize(bufferSize);
    ssize_t bytes_received = read(sockfd, buffer.data(), bufferSize);

    if (bytes_received <= 0) {
        LOGGER_ERROR(kModuleName, "Failed to receive data: {}", strerror(errno));
        disconnectFromServer();
        return false;
    }

    buffer.resize(bytes_received);
    LOGGER_INFO(kModuleName, "Received {} bytes from the server.", bytes_received);

    return true;
}