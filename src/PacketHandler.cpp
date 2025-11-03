#include "PacketHandler.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>

PacketHandler::PacketHandler(const std::unordered_map<int, std::string>& fdRoles,
                             const ConfigManager& config,
                             int txFd)
    : fdRoleMap(fdRoles), configRef(config), tx_fd(txFd) {}

void PacketHandler::handlePacket(const std::vector<uint8_t>& data,
                                 int fd,
                                 const sockaddr_in& clientAddr,
                                 socklen_t addrLen) {
    if (data.size() < HeaderPacket::SIZE) {
        std::cerr << "[PacketHandler] Invalid packet size: " << data.size() << std::endl;
        return;
    }

    HeaderPacket header = HeaderPacket::deserialize(data);
    std::vector<uint8_t> payload(data.begin() + HeaderPacket::SIZE, data.end());

    std::cout << "[PacketHandler] Received packet from role: " << fdRoleMap[fd] << std::endl;
    header.print();

    routePacket(header, payload);
}

void PacketHandler::routePacket(const HeaderPacket& header,
                                const std::vector<uint8_t>& payload) {
    SocketConfig destCfg = configRef.getDestination(header.getDestId(), "tx");
    if (destCfg.port == -1) {
        std::cerr << "[PacketHandler] Destination not found for ID: "
                  << std::string(header.getDestId(), 4) << std::endl;
        return;
    }

    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destCfg.port);
    inet_pton(AF_INET, destCfg.ip, &destAddr.sin_addr);

    int sent = sendto(tx_fd, payload.data(), payload.size(), 0,
                      (sockaddr*)&destAddr, sizeof(destAddr));
    if (sent < 0) {
        perror("sendto");
    } else {
        std::cout << "[PacketHandler] Sent payload to " << destCfg.ip
                  << ":" << destCfg.port << " (" << sent << " bytes)" << std::endl;
    }
}
