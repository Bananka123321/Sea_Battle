#pragma once

#include <string>
#include <iostream>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

const uint32_t MAX_PACKET_SIZE = 1024*1024;

class PacketIO {
public:
    static bool sendPacket(int sock, const std::string& data);
    static bool recvPacket(int sock, std::string& data);

private:
    static bool sendAll(int sock, const void* data, size_t size);
    static bool recvAll(int sock, void* buffer, size_t size);
};