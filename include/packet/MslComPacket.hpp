#ifndef MSL_COM_PACKET_HPP
#define MSL_COM_PACKET_HPP

#include "HeaderPacket.hpp"

class MslComPacket {
    private:
        HeaderPacket header;
        char command_type;

    public:
        MslComPacket() = default;
        MslComPacket(HeaderPacket hdr, char cmd_type)
            : header(hdr), command_type(cmd_type) {}

        ~MslComPacket() = default;
        MslComPacket deserialize(const uint8_t* data);
        void serialize(uint8_t* buffer);
        void print() const {
            std::cout << "MslComPacket(";
            header.print();
            std::cout << "command_type=" << command_type << ")";
            std::cout << std::endl;
        }
};

#endif