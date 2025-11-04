#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <string>

struct SocketConfig {
    char id[5]{};
    char role[10]{};
    char ip[16]{};
    int port = -1;
};

class ConfigManager {
public:
    bool load(const char* filepath);
    std::unordered_map<std::string, SocketConfig> getConfigMapById(const char* id) const;
    SocketConfig getDestination(const char* id, const char* role) const;

private:
    std::vector<SocketConfig> allConfigs;
};

#endif
