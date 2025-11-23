#include "packet/MslKeyPacket.hpp"
#include <iostream>
#include <stdexcept>
#include <iomanip>

MslKeyPacket::MslKeyPacket() = default;

MslKeyPacket::MslKeyPacket(const HeaderPacket& hdr,
                           const std::array<uint8_t, MSL_KEY_SIZE>& keyBytes)
    : header(hdr), key(keyBytes) {}

std::vector<uint8_t> MslKeyPacket::serialize() const {
    std::vector<uint8_t> buffer = header.serialize();
    buffer.reserve(MSL_KEY_PACKET_SIZE);

    buffer.insert(buffer.end(), key.begin(), key.end());

    return buffer;
}

MslKeyPacket MslKeyPacket::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < MSL_KEY_PACKET_SIZE) {
        throw std::runtime_error("Buffer too small for MslKeyPacket");
    }

    std::vector<uint8_t> headerBuf(buffer.begin(),
                                   buffer.begin() + HEADER_PACKET_SIZE);
    HeaderPacket hdr = HeaderPacket::deserialize(headerBuf);

    std::array<uint8_t, MSL_KEY_SIZE> keyBytes{};
    for (size_t i = 0; i < MSL_KEY_SIZE; ++i) {
        keyBytes[i] = buffer[HEADER_PACKET_SIZE + i];
    }

    return MslKeyPacket(hdr, keyBytes);
}

void MslKeyPacket::print() const {
    header.print();

    std::cout << "Key: ";
    for (auto b : key) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << std::endl;
}

const HeaderPacket& MslKeyPacket::getHeader() const {
    return header;
}

const std::array<uint8_t, MSL_KEY_SIZE>& MslKeyPacket::getKey() const {
    return key;
}
