#include "packet/TgtInfoPacket.hpp"
#include "packet/MslInfoPacket.hpp"
#include "packet/MslCmdPacket.hpp"
#include "packet/MslKeyPacket.hpp"
#include "PacketHandler.hpp"
#include "SecurityHandler.hpp"
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
        pkt.print();

        std::string mslId(pkt.getHeader().getDestId(), 4);

        auto it = mslKeyMap.find(mslId);
        if (it == mslKeyMap.end()) {
            std::cerr << "[PacketHandler] No Key For " << mslId << std::endl;
            return;
        }
        const auto& key = it->second;

        //std::vector<uint8_t> encrypted = encryptPayload(data, key);
        std::vector<uint8_t> encrypted = SecurityHandler::encryptPayload(data, key);

        HeaderPacket hdr = HeaderPacket::deserialize(
        std::vector<uint8_t>(encrypted.begin(), encrypted.begin() + HEADER_PACKET_SIZE));

        routePacket(hdr, encrypted, recvRole);

    } 
    else if (std::strcmp(recvRole, "msl_info") == 0) {
        HeaderPacket pkt = HeaderPacket::deserialize(data);
        std::string mslId(pkt.getSrcId(), 4);

        auto it = mslKeyMap.find(mslId);
        if (it == mslKeyMap.end()) {
            std::cerr << "[PacketHandler] No Key For " << mslId << std::endl;
            return;
        }
        const auto& key = it->second;

        //std::vector<uint8_t> decrypted = decryptPayload(data, key);
        std::vector<uint8_t> decrypted = SecurityHandler::decryptPayload(data, key);

        routePacket(pkt, decrypted, recvRole);
    } 
    else if (std::strcmp(recvRole, "msl_cmd") == 0) {
        MslCmdPacket pkt = MslCmdPacket::deserialize(data);
        routePacket(pkt.getHeader(), data, recvRole);
    } 
    else if (std::strcmp(recvRole, "msl_key") == 0) {
        MslKeyPacket pkt = MslKeyPacket::deserialize(data);
        mslKeyMap[std::string(pkt.getMslId(), 4)] = pkt.getKey();
        pkt.print();
    }
    else {
        std::cerr << "[PacketHandler] Unknown role: " << recvRole << std::endl;
    }
}

void PacketHandler::routePacket(const HeaderPacket& header,
                                const std::vector<uint8_t>& packet,
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

    int sent = sendto(tx_fd, packet.data(), packet.size(), 0,
                      (sockaddr*)&destAddr, sizeof(destAddr));
    if (sent < 0) perror("sendto");
}
