#include "ConfigManager.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

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

        // 안전하게 복사하고 널 종료 보장
        std::strncpy(cfg.id, id_buf, sizeof(cfg.id));
        cfg.id[sizeof(cfg.id) - 1] = '\0';

        std::strncpy(cfg.role, role_buf, sizeof(cfg.role));
        cfg.role[sizeof(cfg.role) - 1] = '\0';

        std::strncpy(cfg.ip, ip_buf, sizeof(cfg.ip));
        cfg.ip[sizeof(cfg.ip) - 1] = '\0';

        cfg.port = port;

        // 디버깅 출력
        std::cout << "[LOAD] id=" << cfg.id << ", role=" << cfg.role
                  << ", port=" << cfg.port << ", ip=" << cfg.ip << std::endl;

        allConfigs.push_back(cfg);
    }

    return true;
}

std::unordered_map<std::string, SocketConfig> ConfigManager::getConfigMapById(const char* id) const {
    std::unordered_map<std::string, SocketConfig> result;
    for (const auto& cfg : allConfigs) {
        if (std::strncmp(cfg.id, id, sizeof(cfg.id)) == 0) {
            result[std::string(cfg.role)] = cfg;
        }
    }
    return result;
}

SocketConfig ConfigManager::getDestination(const char* id, const char* role) const {
    std::cout << "[GET DEST] id=" << id << ", role=" << role << std::endl;
    for (const auto& cfg : allConfigs) {
        if (std::strcmp(cfg.id, id) == 0 &&
            std::strcmp(cfg.role, role) == 0) {
            return cfg;
        }
    }
    return SocketConfig();
}
