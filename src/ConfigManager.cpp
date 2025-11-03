#include "ConfigManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdio>

bool ConfigManager::load(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filepath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        SocketConfig cfg;
        char id_buf[5], role_buf[10], ip_buf[16];
        int port;

        int matched = std::sscanf(line.c_str(), "%4s %9s %d %15s", id_buf, role_buf, &port, ip_buf);
        if (matched != 4) {
            std::cerr << "Invalid config line: " << line << std::endl;
            continue;
        }

        std::strncpy(cfg.id, id_buf, sizeof(cfg.id) - 1);
        std::strncpy(cfg.role, role_buf, sizeof(cfg.role) - 1);
        std::strncpy(cfg.ip, ip_buf, sizeof(cfg.ip) - 1);
        cfg.port = port;

        allConfigs.push_back(cfg);
    }

    return true;
}

std::unordered_map<std::string, SocketConfig> ConfigManager::getConfigMapById(const char* id) const {
    std::unordered_map<std::string, SocketConfig> result;
    for (const auto& cfg : allConfigs) {
        if (std::strncmp(cfg.id, id, sizeof(cfg.id)) == 0) {
            result[cfg.role] = cfg;
        }
    }
    return result;
}

SocketConfig ConfigManager::getDestination(const char* id, const char* role) const {
    for (const auto& cfg : allConfigs) {
        if (std::strncmp(cfg.id, id, sizeof(cfg.id)) == 0 &&
            std::strncmp(cfg.role, role, sizeof(cfg.role)) == 0) {
            return cfg;
        }
    }
    return SocketConfig();
}
