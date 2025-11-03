#include "PacketHandler.hpp"
#include "packet/HeaderPacket.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>

void PacketHandler::handlePacket(const std::vector<uint8_t>& data,
                                 int fd,
                                 const sockaddr_in& clientAddr,
                                 socklen_t addrLen)
{
    try {
        HeaderPacket header = HeaderPacket::deserialize(data);
        header.print();

        // 예시 응답 패킷 생성
        HeaderPacket responseHeader(
            'D', '2',
            header.getDestType(),
            header.getDestId(),
            header.getSeq(),
            9
        );

        std::vector<uint8_t> responseData = responseHeader.serialize();

        int sent = sendto(fd, responseData.data(), responseData.size(), 0,
                          (const sockaddr*)&clientAddr, addrLen);
        if (sent < 0) {
            perror("sendto failed");
        } else {
            std::cout << "Response sent successfully." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "PacketHandler error: " << e.what() << std::endl;
    }
}
