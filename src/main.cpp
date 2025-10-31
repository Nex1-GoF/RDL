#include <iostream>
#include "packet/HeaderPacket.hpp"
#include "SocketManager.hpp"
#include "EpollManager.hpp"
using namespace std;

int main() {

    std::cout << "Hello, CMake" << std::endl;

    // 패킷 직렬화, 역직렬화 테스트
    HeaderPacket header('A', '1', 'B', '2', 1, 9);
    
    uint8_t buffer[9];
    header.serialize(buffer);

    HeaderPacket new_header = header.deserialize(buffer);
    new_header.print();

    // 소켓 생성 테스트 
    SocketManager socketManager;
    EpollManager epollManager;
    cout << "epoll creation try " << "\n";


    epollManager.addFd(socketManager.get_tx_fd());
    epollManager.addFd(socketManager.get_msl_info_fd());
    epollManager.addFd(socketManager.get_msl_com_fd());
    epollManager.addFd(socketManager.get_tgt_info_fd());
    epollManager.addFd(socketManager.get_src_fd());

    const char* hello = "Hello from server";
    epollManager.waitAndHandle(hello);
    
    return 0;

}