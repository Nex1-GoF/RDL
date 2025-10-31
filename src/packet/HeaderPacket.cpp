#include "packet/HeaderPacket.hpp"

HeaderPacket::HeaderPacket(char s_type, char s_id, char d_type, char d_id, uint32_t s, uint8_t size)
    : src_type(s_type), src_id(s_id), dest_type(d_type), dest_id(d_id), seq(s), msg_size(size) {}

std::vector<uint8_t> HeaderPacket::serialize() const {
    std::vector<uint8_t> buffer(SIZE);
    buffer[0] = static_cast<uint8_t>(src_type);
    buffer[1] = static_cast<uint8_t>(src_id);
    buffer[2] = static_cast<uint8_t>(dest_type);
    buffer[3] = static_cast<uint8_t>(dest_id);
    buffer[4] = (seq >> 24) & 0xFF;
    buffer[5] = (seq >> 16) & 0xFF;
    buffer[6] = (seq >> 8) & 0xFF;
    buffer[7] = seq & 0xFF;
    buffer[8] = msg_size;
    return buffer;
}

HeaderPacket HeaderPacket::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < SIZE) {
        throw std::invalid_argument("Buffer too small for HeaderPacket");
    }

    uint32_t seq_val = (static_cast<uint32_t>(buffer[4]) << 24) |
                       (static_cast<uint32_t>(buffer[5]) << 16) |
                       (static_cast<uint32_t>(buffer[6]) << 8) |
                       static_cast<uint32_t>(buffer[7]);

    return HeaderPacket(
        static_cast<char>(buffer[0]),
        static_cast<char>(buffer[1]),
        static_cast<char>(buffer[2]),
        static_cast<char>(buffer[3]),
        seq_val,
        buffer[8]
    );
}

void HeaderPacket::print() const {
    std::cout << "HeaderPacket("
              << "src_type=" << src_type << ", "
              << "src_id=" << src_id << ", "
              << "dest_type=" << dest_type << ", "
              << "dest_id=" << dest_id << ", "
              << "seq=" << seq << ", "
              << "msg_size=" << static_cast<unsigned>(msg_size)
              << ")" << std::endl;
}

char HeaderPacket::getDestType() const { return dest_type; }
char HeaderPacket::getDestId() const { return dest_id; }
uint32_t HeaderPacket::getSeq() const { return seq; }
