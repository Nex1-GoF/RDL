// #include "packet/TgtInfoPacket.hpp"
// using namespace std;

// void TgtInfoPacket::serialize(uint8_t* buffer) const {
//     header.serialize(buffer);
//     memcpy(buffer + 9, id.data(), 4);
//     buffer[13] = (x >> 24) & 0xFF;
//     buffer[14] = (x >> 16) & 0xFF;
//     buffer[15] = (x >> 8) & 0xFF;
//     buffer[16] = x & 0xFF;
//     buffer[17] = (y >> 24) & 0xFF;
//     buffer[18] = (y >> 16) & 0xFF;
//     buffer[19] = (y >> 8) & 0xFF;
//     buffer[20] = y & 0xFF;
//     buffer[21] = (z >> 24) & 0xFF;
//     buffer[22] = (z >> 16) & 0xFF;
//     buffer[23] = (z >> 8) & 0xFF;
//     buffer[24] = z & 0xFF;
//     buffer[25] = (vx >> 8) & 0xFF;
//     buffer[26] = vx & 0xFF;
//     buffer[27] = (vy >> 8) & 0xFF;
//     buffer[28] = vy & 0xFF;
//     buffer[29] = (vz >> 8) & 0xFF;
//     buffer[30] = vz & 0xFF;
//     buffer[31] = detected_type;
//     buffer[32] = (detected_time >> 24) & 0xFF;
//     buffer[33] = (detected_time >> 16) & 0xFF;
//     buffer[34] = (detected_time >> 8) & 0xFF;
//     buffer[35] = detected_time & 0xFF;
// }

// TgtInfoPacket TgtInfoPacket::deserialize(const uint8_t* data) {
//     header = header.deserialize(data);
//     memcpy(id.data(), data + 9, 4);
//     x = (data[13] << 24) | (data[14] << 16) | (data[15] << 8) | data[16];
//     y = (data[17] << 24) | (data[18] << 16) | (data[19] << 8) | data[20];
//     z = (data[21] << 24) | (data[22] << 16) | (data[23] << 8) | data[24];
//     vx = (data[25] << 8) | data[26];
//     vy = (data[27] << 8) | data[28];
//     vz = (data[29] << 8) | data[30];
//     detected_type = data[31];
//     detected_time = (data[32] << 24) | (data[33] << 16) | (data[34] << 8) | data[35];
//     return *this;
// }

