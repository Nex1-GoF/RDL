#include "ConfigManager.hpp"
#include <fstream>
#include <sstream>

bool ConfigManager::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, ip;
        int port;

        if (!(iss >> name >> port >> ip)) continue;

        socketConfigs[name] = { port, ip };
    }

    return true;
}

SocketConfig ConfigManager::getSocketConfig(const std::string& key) const {
    auto it = socketConfigs.find(key);
    if (it != socketConfigs.end()) {
        return it->second;
    }
    return { -1, "" };
}
