#include "packet/HeaderPacket.hpp"
using namespace std;

void HeaderPacket::serialize(uint8_t* buffer) {
    buffer[0] = src_type;
    buffer[1] = src_id;
    buffer[2] = dest_type;
    buffer[3] = dest_id;
    buffer[4] = (seq >> 24) & 0xFF;
    buffer[5] = (seq >> 16) & 0xFF;
    buffer[6] = (seq >> 8) & 0xFF;
    buffer[7] = seq & 0xFF;
    buffer[8] = msg_size;
}

HeaderPacket HeaderPacket::deserialize(const uint8_t* data) {
    src_type = data[0];
    src_id = data[1];
    dest_type = data[2];
    dest_id = data[3];
    seq = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
    msg_size = data[8];

    return *this;
}