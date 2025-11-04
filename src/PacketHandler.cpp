#include "packet/TgtInfoPacket.hpp"
#include "PacketHandler.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>

PacketHandler::PacketHandler(const std::unordered_map<int, const char*>& fdRoles,
                             const ConfigManager& config,
                             int txFd)
    : fdRoleMap(fdRoles), configRef(config), tx_fd(txFd) {}

void PacketHandler::handlePacket(const std::vector<uint8_t>& data,
                                 int fd,
                                 const sockaddr_in& clientAddr,
                                 socklen_t addrLen) {
    const char* recvRole = fdRoleMap[fd];

    if (std::strcmp(recvRole, "tgt_info") == 0) {
        TgtInfoPacket pkt = TgtInfoPacket::deserialize(data);
        const HeaderPacket& header = pkt.getHeader();
        routePacket(header, data, recvRole);  // 원본 그대로 전송
    } else {
        std::cerr << "[PacketHandler] Unknown role: " << recvRole << std::endl;
    }
}

void PacketHandler::routePacket(const HeaderPacket& header,
                                const std::vector<uint8_t>& payload,
                                const char* recvRole) {
    SocketConfig destCfg = configRef.getDestination(header.getDestId(), recvRole);
    std::cout << destCfg.id << " " << destCfg.role << " " << destCfg.ip << " " << destCfg.port << std::endl;
    if (destCfg.port == -1) {
        std::cerr << "[PacketHandler] No destination for " << std::string(header.getDestId(), 4)
                  << " with role " << recvRole << std::endl;
        return;
    }

    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destCfg.port);
    inet_pton(AF_INET, destCfg.ip, &destAddr.sin_addr);

    int sent = sendto(tx_fd, payload.data(), payload.size(), 0,
                      (sockaddr*)&destAddr, sizeof(destAddr));
    if (sent < 0) perror("sendto");
}
