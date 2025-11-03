#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <string>
#include <vector>

struct SocketConfig {
    std::string id;       // 시스템 ID (예: D001, M001)
    std::string role;     // 역할 (예: tx, msl_info, tgt_info)
    int port;             // 포트 번호
    std::string ip;       // IP 주소
};

class ConfigManager {
public:
    bool load(const std::string& filepath);

    // 내 시스템 ID에 해당하는 바인딩 정보 조회
    std::vector<SocketConfig> getConfigsById(const std::string& id) const;

    // 특정 시스템 ID + 역할에 해당하는 전송 대상 조회
    SocketConfig getDestination(const std::string& id, const std::string& role) const;

private:
    std::vector<SocketConfig> allConfigs;
};

#endif