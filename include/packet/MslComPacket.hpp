#ifndef __MslComPacket_hpp__
#define __MslComPacket_hpp__

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
};