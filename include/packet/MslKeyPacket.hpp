#ifndef MSL_KEY_PACKET_HPP
#define MSL_KEY_PACKET_HPP

#include "HeaderPacket.hpp"
#include <cstdint>
#include <vector>
#include <array>

constexpr size_t MSL_KEY_SIZE = 32;
constexpr size_t MSL_KEY_PACKET_SIZE = HEADER_PACKET_SIZE + 32;

class MslKeyPacket {
private:
    HeaderPacket header;
    std::array<uint8_t, MSL_KEY_SIZE> key;

public:
    MslKeyPacket();
    MslKeyPacket(const HeaderPacket& hdr,
                 const std::array<uint8_t, MSL_KEY_SIZE>& keyBytes);

    std::vector<uint8_t> serialize() const;
    static MslKeyPacket deserialize(const std::vector<uint8_t>& buffer);

    void print() const;

    const HeaderPacket& getHeader() const;
    const std::array<uint8_t, MSL_KEY_SIZE>& getKey() const;
};

#endif
