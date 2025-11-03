#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include <vector>
#include <cstdint>
#include <netinet/in.h>

class PacketHandler {
public:
    void handlePacket(const std::vector<uint8_t>& data,
                      int fd,
                      const sockaddr_in& clientAddr,
                      socklen_t addrLen);
};

#endif