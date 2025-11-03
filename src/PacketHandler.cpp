#include "PacketHandler.hpp"
#include "packet/HeaderPacket.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

PacketHandler::PacketHandler(const std::unordered_map<int, std::string>& fdRoles,
                             const ConfigManager& config)
    : fdRole(fdRoles), configRef(config) {}

void PacketHandler::handlePacket(const std::vector<uint8_t>& data,
                                 int fd,
                                 const sockaddr_in& clientAddr,
                                 socklen_t addrLen)
{
    std::string role = fdRole.count(fd) ? fdRole.at(fd) : "unknown";
    std::cout << "[PacketHandler] Received on fd " << fd << " (" << role << ")" << std::endl;

    try {
        HeaderPacket header = HeaderPacket::deserialize(data);
        header.print();

        if (role == "tgt_info") {
            std::string destId = "M00" + std::string(1, header.getDestId());
            SocketConfig dest = configRef.getDestination(destId, "tgt_info");

            sockaddr_in targetAddr{};
            targetAddr.sin_family = AF_INET;
            targetAddr.sin_port = htons(dest.port);
            inet_pton(AF_INET, dest.ip.c_str(), &targetAddr.sin_addr);

            std::vector<uint8_t> responseData = header.serialize(); // 예시 응답
            sendto(fd, responseData.data(), responseData.size(), 0,
                   (sockaddr*)&targetAddr, sizeof(targetAddr));

            std::cout << "Sent to " << dest.ip << ":" << dest.port << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "PacketHandler error: " << e.what() << std::endl;
    }
}
