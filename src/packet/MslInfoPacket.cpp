#include "MslInfoPacket.hpp"
using namespace std;

void MslInfoPacket::serialize(uint8_t* buffer) const {
    header.serialize(buffer);
    buffer[9] = (latitude >> 24) & 0xFF;
    buffer[10] = (latitude >> 16) & 0xFF;
    buffer[11] = (latitude >> 8) & 0xFF;
    buffer[12] = latitude & 0xFF;
    buffer[13] = (longitude >> 24) & 0xFF;
    buffer[14] = (longitude >> 16) & 0xFF;
    buffer[15] = (longitude >> 8) & 0xFF;
    buffer[16] = longitude & 0xFF;
    buffer[17] = (altitude >> 8) & 0xFF;
    buffer[18] = altitude & 0xFF;
    buffer[19] = (yaw >> 8) & 0xFF;
    buffer[20] = yaw & 0xFF;
    buffer[21] = (pitch >> 8) & 0xFF;
    buffer[22] = pitch & 0xFF;
    buffer[23] = (flight_time >> 24) & 0xFF;
    buffer[24] = (flight_time >> 16) & 0xFF;
    buffer[25] = (flight_time >> 8) & 0xFF;
    buffer[26] = flight_time & 0xFF;
    buffer[27] = status;
    buffer[28] = telemetry;
}

MslInfoPacket MslInfoPacket::deserialize(const uint8_t* data) {
    header = header.deserialize(data);
    latitude = (data[9] << 24) | (data[10] << 16) | (data[11] << 8) | data[12];
    longitude = (data[13] << 24) | (data[14] << 16) | (data[15] << 8) | data[16];
    altitude = (data[17] << 8) | data[18];
    yaw = (data[19] << 8) | data[20];
    pitch = (data[21] << 8) | data[22];
    flight_time = (data[23] << 24) | (data[24] << 16) | (data[25] << 8) | data[26];
    status = data[27];
    telemetry = data[28];
    return *this;
}
