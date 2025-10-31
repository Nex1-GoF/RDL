#include <iostream>
#include "packet/HeaderPacket.hpp"
#include "Network.hpp"

int main() {

    std::cout << "Hello, CMake" << std::endl;

    // 패킷 직렬화, 역직렬화 테스트
    HeaderPacket header('A', '1', 'B', '2', 1, 9);
    
    uint8_t buffer[9];
    header.serialize(buffer);

    HeaderPacket new_header = header.deserialize(buffer);
    new_header.print();

    // 소켓 생성 테스트 
    Network network;
    network.setup_sockets();
    
    return 0;
}