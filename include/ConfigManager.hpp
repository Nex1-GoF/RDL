#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstring>

struct SocketConfig {
    char id[5];     // 시스템 ID (예: "D001")
    char role[10];  // 역할 (예: "tx", "msl_info")
    int port;       // 포트 번호
    char ip[16];    // IPv4 주소

    SocketConfig() {
        std::memset(id, 0, sizeof(id));
        std::memset(role, 0, sizeof(role));
        std::memset(ip, 0, sizeof(ip));
        port = -1;
    }
};

class ConfigManager {
public:
    bool load(const char* filepath);

    // 특정 시스템 ID에 해당하는 역할별 소켓 설정 맵 반환
    std::unordered_map<std::string, SocketConfig> getConfigMapById(const char* id) const;

    // 특정 시스템 ID + 역할에 해당하는 단일 소켓 설정 반환
    SocketConfig getDestination(const char* id, const char* role) const;

private:
    std::vector<SocketConfig> allConfigs;
};

#endif
