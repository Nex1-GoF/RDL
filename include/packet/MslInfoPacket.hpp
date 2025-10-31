// #ifndef MSL_INFO_PACKET_HPP
// #define MSL_INFO_PACKET_HPP

// #include "HeaderPacket.hpp"

// class MslInfoPacket{
//     private:
//         HeaderPacket header;
//         int32_t latitude;
//         int32_t longitude;
//         int16_t altitude;
//         int16_t yaw;
//         int16_t pitch;
//         uint32_t flight_time;
//         char status;
//         char telemetry;

//     public:
//         MslInfoPacket() = default;
//         MslInfoPacket(HeaderPacket hdr, int32_t lat, int32_t lon,
//                       int16_t alt, int16_t yw, int16_t pt,
//                       uint32_t f_time, char stat, char telem)
//             : header(hdr), latitude(lat), longitude(lon),
//               altitude(alt), yaw(yw), pitch(pt),
//               flight_time(f_time), status(stat), telemetry(telem) {}
//         ~MslInfoPacket() = default;
//         MslInfoPacket deserialize(const uint8_t* data);
//         void serialize(uint8_t* buffer);
//         void print() const {
//             std::cout << "MslInfoPacket(";
//             header.print();
//             std::cout << "latitude=" << latitude << ", "
//                       << "longitude=" << longitude << ", "
//                       << "altitude=" << altitude << ", "
//                       << "yaw=" << yaw << ", "
//                       << "pitch=" << pitch << ", "
//                       << "flight_time=" << flight_time << ", "
//                       << "status=" << static_cast<unsigned>(status) << ", "
//                       << "telemetry=" << static_cast<unsigned>(telemetry)
//                       << ")" << std::endl;
//         }
// };

// #endif