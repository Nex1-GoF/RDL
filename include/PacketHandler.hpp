#ifndef PACKET_HANDLER_HPP
#define PACKET_HANDLER_HPP

#include <vector>
#include <cstdint>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <string>
#include "ConfigManager.hpp"
#include "packet/HeaderPacket.hpp"

class PacketHandler {
public:
    PacketHandler(const std::unordered_map<int, std::string>& fdRoles,
                  const ConfigManager& config,
                  int txFd);

    void handlePacket(const std::vector<uint8_t>& data,
                      int fd,
                      const sockaddr_in& clientAddr,
                      socklen_t addrLen);

private:
    std::unordered_map<int, std::string> fdRoleMap;
    const ConfigManager& configRef;
    int tx_fd;

    void routePacket(const HeaderPacket& header,
                     const std::vector<uint8_t>& payload);
};

#endif
