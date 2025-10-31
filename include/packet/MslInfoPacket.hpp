#ifndef __MslInfoPacket_hpp__
#define __MslInfoPacket_hpp__

class MslInfoPacket{
    private:
        HeaderPacket header;
        int32_t latitude;
        int32_t longitude;
        int16_t altitude;
        int16_t yaw;
        int16_t pitch;
        uint32_t flight_time;
        char status;
        char telemetry;

    public:
        MslInfoPacket() = default;
        MslInfoPacket(HeaderPacket hdr, int32_t lat, int32_t lon,
                      int16_t alt, int16_t yw, int16_t pt,
                      uint32_t f_time, char stat, char telem)
            : header(hdr), latitude(lat), longitude(lon),
              altitude(alt), yaw(yw), pitch(pt),
              flight_time(f_time), status(stat), telemetry(telem) {}

        ~MslInfoPacket() = default;
        MslInfoPacket deserialize(const uint8_t* data);
        void serialize(uint8_t* buffer);
}