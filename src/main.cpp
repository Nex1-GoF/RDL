#include <iostream>
#include "packet/HeaderPacket.hpp"
#include "SocketManager.hpp"
#include "EpollManager.hpp"
#include "ConfigManager.hpp"
#include <iostream>
using namespace std;

int main() {

    std::cout << "Hello, CMake" << std::endl;

    // 패킷 직렬화, 역직렬화 테스트
    // HeaderPacket header('A', '1', 'B', '2', 1, 9);
    
    // uint8_t buffer[9];
    // header.serialize(buffer);

    // HeaderPacket new_header = header.deserialize(buffer);
    // new_header.print();

    // 설정 파일 로드
    ConfigManager config;
    if (!config.load("/home/user/Project/RDL/config_local.txt")) {
        std::cerr << "Failed to load config file" << std::endl;
        return 1;
    }

    std::string myId = "D001"; // 현재 시스템 ID

    // 소켓 설정
    SocketManager socketManager;
    socketManager.setup_sockets(config, myId);

    std::unordered_map<int, std::string> fdRoles;
    for (const std::string& role : { "tx", "msl_info", "msl_com", "tgt_info", "src" }) {
        int fd = socketManager.get_fd_by_role(role);
        if (fd != -1) fdRoles[fd] = role;
    }

    PacketHandler handler(fdRoles, config);

    EpollManager epoll;
    for (const auto& [fd, role] : fdRoles) {
        epoll.addFd(fd);
    }

    epoll.waitAndHandle(handler);
    return 0;

}