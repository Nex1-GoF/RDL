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

    // 소켓 설정
    SocketManager socketManager;
    socketManager.setup_sockets(config);

    // epoll 등록
    EpollManager epollManager;

    epollManager.setTxFd(socketManager.get_tx_fd()); 

    epollManager.addFd(socketManager.get_msl_info_fd());
    epollManager.addFd(socketManager.get_msl_com_fd());
    epollManager.addFd(socketManager.get_tgt_info_fd());
    epollManager.addFd(socketManager.get_src_fd());

    epollManager.waitAndHandle();
    
    return 0;

}