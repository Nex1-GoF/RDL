#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <string>
#include <unordered_map>

struct SocketConfig {
    int port;
    std::string ip;
};

class ConfigManager {
public:
    bool load(const std::string& filepath);
    SocketConfig getSocketConfig(const std::string& key) const;

private:
    std::unordered_map<std::string, SocketConfig> socketConfigs;
};

#endif