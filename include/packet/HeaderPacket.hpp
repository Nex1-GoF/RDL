#ifndef __HEADER_PACKET_HPP__
#define __HEADER_PACKET_HPP__

#include <cstdint>       
#include <iostream>       

class HeaderPacket {            
    private:
        char src_type;
        char src_id;
        char dest_type;
        char dest_id;               
        uint32_t seq;
        uint8_t msg_size;

    public:
        HeaderPacket() = default;
        HeaderPacket(char s_type, char s_id,char d_type, char d_id,
                    uint32_t s, uint8_t size)
            : src_type(s_type), src_id(s_id), dest_type(d_type), dest_id(d_id),
            seq(s), msg_size(size) {}
        ~HeaderPacket() = default;
        HeaderPacket deserialize(const uint8_t* data);
        void serialize(uint8_t* buffer);
        void print() const {
        std::cout << "HeaderPacket("
                  << "src_type=" << src_type << ", "
                  << "src_id=" << src_id << ", "
                  << "dest_type=" << dest_type << ", "
                  << "dest_id=" << dest_id << ", "
                  << "seq=" << seq << ", "
                  << "msg_size=" << static_cast<unsigned>(msg_size)
                  << ")" << std::endl;
        }
        char getDestType() const {
            return dest_type;
        }
        char getDestId() const {
            return dest_id;
        }
        uint32_t getSeq() const {
            return seq;
        }
};

#endif
