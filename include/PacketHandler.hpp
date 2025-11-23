#ifndef PACKET_HANDLER_HPP
#define PACKET_HANDLER_HPP

#include "ConfigManager.hpp"
#include "packet/HeaderPacket.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/aes.h>  


class PacketHandler {
public:
    PacketHandler(const std::unordered_map<int, const char*>& fdRoles,
                  const ConfigManager& config,
                  int txFd);

    void handlePacket(const std::vector<uint8_t>& data,
                      int fd,
                      const sockaddr_in& clientAddr,
                      socklen_t addrLen);

private:
    void routePacket(const HeaderPacket& header,
                     const std::vector<uint8_t>& payload,
                     const char* recvRole);

    std::vector<uint8_t> addPkcs7Padding(const std::vector<uint8_t>& plain);

    void aes256EcbEncrypt(const std::vector<uint8_t>& plain,
                          std::vector<uint8_t>& cipher,
                          const std::array<uint8_t, 32>& key);

    std::vector<uint8_t> encryptPayload(const std::vector<uint8_t>& fullPacket,
                                        const std::array<uint8_t, 32>& key);

    std::unordered_map<int, const char*> fdRoleMap;
    const ConfigManager& configRef;
    int tx_fd;

    std::unordered_map<std::string, std::array<uint8_t, 32>> mslKeyMap;
};

#endif
