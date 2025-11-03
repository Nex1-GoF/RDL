#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include <vector>
#include <cstdint>
#include <netinet/in.h>
#include <unordered_map>

class PacketHandler {
public:
    PacketHandler(const std::unordered_map<int, std::string>& fdRoles);
    void handlePacket(const std::vector<uint8_t>& data,
                      int fd,
                      const sockaddr_in& clientAddr,
                      socklen_t addrLen);

private:
    std::unordered_map<int, std::string> fdRole;    
};

#endif