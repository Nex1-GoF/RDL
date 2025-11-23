#include "packet/MslKeyPacket.hpp"
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <cstring>

MslKeyPacket::MslKeyPacket() {
    std::memset(msl_id, 0, sizeof(msl_id));
    key.fill(0);
}

MslKeyPacket::MslKeyPacket(const HeaderPacket& hdr,
                           const char* mslId,
                           const std::array<uint8_t, MSL_KEY_SIZE>& keyBytes)
    : header(hdr), key(keyBytes)
{
    std::memcpy(msl_id, mslId, 4);   // "M001"
    msl_id[4] = '\0';                // 마지막 인덱스 = 4
}

std::vector<uint8_t> MslKeyPacket::serialize() const {
    std::vector<uint8_t> buffer = header.serialize();
    buffer.reserve(MSL_KEY_PACKET_SIZE);

    // mslId (4 bytes)
    buffer.insert(buffer.end(), msl_id, msl_id + 4);

    // key (32 bytes)
    buffer.insert(buffer.end(), key.begin(), key.end());

    return buffer;
}

MslKeyPacket MslKeyPacket::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < MSL_KEY_PACKET_SIZE) {
        throw std::runtime_error("Buffer too small for MslKeyPacket");
    }

    // Header
    std::vector<uint8_t> headerBuf(buffer.begin(),
                                   buffer.begin() + HEADER_PACKET_SIZE);
    HeaderPacket hdr = HeaderPacket::deserialize(headerBuf);

    // mslId (4 bytes)
    char id[5];
    std::memcpy(id, &buffer[HEADER_PACKET_SIZE], 4);
    id[4] = '\0';

    // key (32 bytes)
    std::array<uint8_t, MSL_KEY_SIZE> keyBytes{};
    size_t offset = HEADER_PACKET_SIZE + 4;

    for (size_t i = 0; i < MSL_KEY_SIZE; ++i) {
        keyBytes[i] = buffer[offset + i];
    }

    return MslKeyPacket(hdr, id, keyBytes);
}

void MslKeyPacket::print() const {
    header.print();

    std::cout << "msl_id=" << msl_id << "\nKey: ";
    for (auto b : key) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << std::endl;
}

const HeaderPacket& MslKeyPacket::getHeader() const {
    return header;
}

const char* MslKeyPacket::getMslId() const {
    return msl_id;
}

const std::array<uint8_t, MSL_KEY_SIZE>& MslKeyPacket::getKey() const {
    return key;
}
