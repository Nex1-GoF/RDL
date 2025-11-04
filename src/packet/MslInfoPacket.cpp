#include "packet/MslInfoPacket.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

MslInfoPacket::MslInfoPacket() = default;

MslInfoPacket::MslInfoPacket(const HeaderPacket& hdr,
                             int32_t lat, int32_t lon, int16_t alt,
                             int16_t yaw_, int16_t pitch_,
                             uint32_t time, char f_status, char t_status)
    : header(hdr), latitude(lat), longitude(lon), altitude(alt),
      yaw(yaw_), pitch(pitch_), flight_time(time),
      flight_status(f_status), telemetry_status(t_status) {}

std::vector<uint8_t> MslInfoPacket::serialize() const {
    std::vector<uint8_t> buffer = header.serialize();

    std::vector<uint8_t> body(MSL_INFO_BODY_SIZE);
    std::memcpy(&body[0], &latitude, sizeof(latitude));
    std::memcpy(&body[4], &longitude, sizeof(longitude));
    std::memcpy(&body[8], &altitude, sizeof(altitude));
    std::memcpy(&body[10], &yaw, sizeof(yaw));
    std::memcpy(&body[12], &pitch, sizeof(pitch));
    std::memcpy(&body[14], &flight_time, sizeof(flight_time));
    body[18] = static_cast<uint8_t>(flight_status);
    body[19] = static_cast<uint8_t>(telemetry_status);

    buffer.insert(buffer.end(), body.begin(), body.begin() + 20); // 실제 사용은 20바이트
    return buffer;
}

MslInfoPacket MslInfoPacket::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < MSL_INFO_PACKET_SIZE)
        throw std::runtime_error("Buffer too small for MslInfoPacket");

    HeaderPacket hdr = HeaderPacket::deserialize({buffer.begin(), buffer.begin() + HEADER_PACKET_SIZE});
    MslInfoPacket pkt;
    pkt.header = hdr;

    std::memcpy(&pkt.latitude, &buffer[13], sizeof(pkt.latitude));
    std::memcpy(&pkt.longitude, &buffer[17], sizeof(pkt.longitude));
    std::memcpy(&pkt.altitude, &buffer[21], sizeof(pkt.altitude));
    std::memcpy(&pkt.yaw, &buffer[23], sizeof(pkt.yaw));
    std::memcpy(&pkt.pitch, &buffer[25], sizeof(pkt.pitch));
    std::memcpy(&pkt.flight_time, &buffer[27], sizeof(pkt.flight_time));
    pkt.flight_status = static_cast<char>(buffer[31]);
    pkt.telemetry_status = static_cast<char>(buffer[32]);

    return pkt;
}

void MslInfoPacket::print() const {
    std::cout << "[MslInfoPacket]\n";
    header.print();
    std::cout << "Position(lat=" << latitude
              << ", lon=" << longitude
              << ", alt=" << altitude << ")\n"
              << "Attitude(yaw=" << yaw
              << ", pitch=" << pitch << ")\n"
              << "FlightTime=" << flight_time << " ms, "
              << "Status=" << flight_status
              << ", Telemetry=" << telemetry_status << "\n";
}

const HeaderPacket& MslInfoPacket::getHeader() const {
    return header;
}
