#ifndef HEADER_PACKET_HPP
#define HEADER_PACKET_HPP

#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>

class HeaderPacket {
private:
    char src_type;
    char src_id;
    char dest_type;
    char dest_id;
    uint32_t seq;
    uint8_t msg_size;

public:
    static const size_t SIZE = 9;

    HeaderPacket() = default;
    HeaderPacket(char s_type, char s_id, char d_type, char d_id, uint32_t s, uint8_t size);

    std::vector<uint8_t> serialize() const;
    static HeaderPacket deserialize(const std::vector<uint8_t>& buffer);

    void print() const;

    // Getters
    char getDestType() const;
    char getDestId() const;
    uint32_t getSeq() const;
};

#endif
