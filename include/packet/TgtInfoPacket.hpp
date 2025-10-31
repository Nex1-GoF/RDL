// #ifdef TGT_INFO_PACKET_HPP
// #define TGT_INFO_PACKET_HPP

// #include "HeaderPacket.hpp"

// class TgtInfoPacket {
//     private:
//         HeaderPacket header;
//         std::array<char,4> id;
//         int32_t x;
//         int32_t y;
//         int32_t z;
//         int16_t vx;
//         int16_t vy;
//         int16_t vz;
//         char detected_type;
//         uint32_t detected_time;

//     public:
//         TgtInfoPacket() = default;
//         TgtInfoPacket(HeaderPacket hdr, const std::array<char,4>& identifier,
//                       int32_t xpos, int32_t ypos, int32_t zpos,
//                       int16_t xvel, int16_t yvel, int16_t zvel,
//                       char det_type, uint32_t det_time)
//             : header(hdr), id(identifier), x(xpos), y(ypos), z(zpos),
//               vx(xvel), vy(yvel), vz(zvel),
//               detected_type(det_type), detected_time(det_time) {}
//         ~TgtInfoPacket() = default;
//         MslInfoPacket deserialize(const uint8_t* data);
//         void serialize(uint8_t* buffer);
//         void print() const {
//             std::cout << "TgtInfoPacket(";
//             header.print();
//             std::cout << "id=" << std::string(id.data(), id.size()) << ", "
//                       << "x=" << x << ", "
//                       << "y=" << y << ", "
//                       << "z=" << z << ", "
//                       << "vx=" << vx << ", "
//                       << "vy=" << vy << ", "
//                       << "vz=" << vz << ", "
//                       << "detected_type=" << static_cast<unsigned>(detected_type) << ", "
//                       << "detected_time=" << detected_time
//                       << ")" << std::endl;
//         }   
// };



// #endif