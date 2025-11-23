#ifndef MSL_KEY_PACKET_HPP
#define MSL_KEY_PACKET_HPP

#include "HeaderPacket.hpp"
#include <array>
#include <cstdint>
#include <vector>
#include <cstring>

constexpr size_t MSL_KEY_SIZE        = 32;
constexpr size_t MSL_KEY_PACKET_SIZE = HEADER_PACKET_SIZE + 36;

class MslKeyPacket {
public:
    MslKeyPacket();
    MslKeyPacket(const HeaderPacket& hdr,
                 const char* mslId,
                 const std::array<uint8_t, MSL_KEY_SIZE>& keyBytes);

    std::vector<uint8_t> serialize() const;
    static MslKeyPacket deserialize(const std::vector<uint8_t>& buffer);

    void print() const;

    const HeaderPacket& getHeader() const;
    const char* getMslId() const;
    const std::array<uint8_t, MSL_KEY_SIZE>& getKey() const;

private:
    HeaderPacket header;

    char msl_id[5];   // 4 chars + null
    std::array<uint8_t, MSL_KEY_SIZE> key;
};

#endif
