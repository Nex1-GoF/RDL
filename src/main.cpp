#include <iostream>
#include "packet/HeaderPacket.hpp"

int main() {

    std::cout << "Hello, CMake" << std::endl;

    HeaderPacket header('A', '1', 'B', '2', 1, 9);
    
    uint8_t buffer[9];
    header.serialize(buffer);

    HeaderPacket new_header = header.deserialize(buffer);
    new_header.print();

    return 0;
}