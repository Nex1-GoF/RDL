#include "PacketHandler.hpp"
#include "packet/HeaderPacket.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>

PacketHandler::PacketHandler(const std::unordered_map<int, std::string>& fdRoles)
    : fdRole(fdRoles) {}

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

        // 메시지 타입에 따라 분기 처리
        if (role == "msl_info") {
            std::cout << "Processing Missile Info packet..." << std::endl;
            // 미사일 정보 처리
        } 
        else if (role == "msl_com") {
            std::cout << "Processing Missile Command packet..." << std::endl;
            // 명령 처리
        } 
        else if (role == "tgt_info") {
            std::cout << "Processing TGT INFO packet..." << std::endl;
            // TX 처리 로직
        } 
        else {
            std::cout << "Unknown role, default handling..." << std::endl;
        }

        // 응답 예시
        HeaderPacket responseHeader(
            'D', '2',
            header.getDestType(),
            header.getDestId(),
            header.getSeq(),
            9
        );

        std::vector<uint8_t> responseData = responseHeader.serialize();

        sendto(fd, responseData.data(), responseData.size(), 0,
               (const sockaddr*)&clientAddr, addrLen);

    } catch (const std::exception& e) {
        std::cerr << "PacketHandler error: " << e.what() << std::endl;
    }
}
