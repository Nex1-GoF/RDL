#ifndef HEADER_PACKET_HPP
#define HEADER_PACKET_HPP

#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstring>

class HeaderPacket {
private:
    char src_id[5];     // 송신자 ID (예: "D001")
    char dest_id[5];    // 수신자 ID (예: "M001")
    uint32_t seq;       // 시퀀스 번호
    uint8_t msg_size;   // 메시지 크기

public:
    static const size_t SIZE = 13;

    HeaderPacket() = default;
    HeaderPacket(const char* s_id, const char* d_id, uint32_t s, uint8_t size);

    std::vector<uint8_t> serialize() const;
    static HeaderPacket deserialize(const std::vector<uint8_t>& buffer);

    void print() const;

    const char* getSrcId() const;
    const char* getDestId() const;
    uint32_t getSeq() const;
    uint8_t getMsgSize() const;
};

#endif
