#include "MSLComPacket.hpp"
using namespace std;

void MslComPacket::serialize(uint8_t* buffer) const {
    header.serialize(buffer);
    buffer[9] = command_type;
}

MslComPacket MslComPacket::deserialize(const uint8_t* data) {
    header = header.deserialize(data);
    command_type = data[9];
    return *this;
}