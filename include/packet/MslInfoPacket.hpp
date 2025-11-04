#ifndef MSL_INFO_PACKET_HPP
#define MSL_INFO_PACKET_HPP

#include "HeaderPacket.hpp"
#include <cstdint>
#include <vector>
#include <string>

constexpr size_t MSL_INFO_BODY_SIZE = 20;
constexpr size_t MSL_INFO_PACKET_SIZE = HEADER_PACKET_SIZE + MSL_INFO_BODY_SIZE;

class MslInfoPacket {
private:
    HeaderPacket header;

    int32_t latitude;     // 위도 (×1e7)
    int32_t longitude;    // 경도 (×1e7)
    int16_t altitude;     // 고도 (m)
    int16_t yaw;          // 요 (0.01°)
    int16_t pitch;        // 피치 (0.01°)
    uint32_t flight_time; // 비행시간 (ms)
    char flight_status;   // 비행상태 (1~5)
    char telemetry_status;// 텔레메트리 상태 (1~3)

public:
    MslInfoPacket();
    MslInfoPacket(const HeaderPacket& hdr,
                  int32_t lat, int32_t lon, int16_t alt,
                  int16_t yaw_, int16_t pitch_,
                  uint32_t time, char f_status, char t_status);

    std::vector<uint8_t> serialize() const;
    static MslInfoPacket deserialize(const std::vector<uint8_t>& buffer);
    void print() const;

    const HeaderPacket& getHeader() const;
};

#endif
