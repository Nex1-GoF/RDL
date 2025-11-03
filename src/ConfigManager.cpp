#include "ConfigManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool ConfigManager::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filepath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        SocketConfig cfg;
        if (!(iss >> cfg.id >> cfg.role >> cfg.port >> cfg.ip)) {
            std::cerr << "Invalid config line: " << line << std::endl;
            continue;
        }

        allConfigs.push_back(cfg);
    }

    return true;
}

std::vector<SocketConfig> ConfigManager::getConfigsById(const std::string& id) const {
    std::vector<SocketConfig> result;
    for (const auto& cfg : allConfigs) {
        if (cfg.id == id) result.push_back(cfg);
    }
    return result;
}

SocketConfig ConfigManager::getDestination(const std::string& id, const std::string& role) const {
    for (const auto& cfg : allConfigs) {
        if (cfg.id == id && cfg.role == role) return cfg;
    }
    return { "", "", -1, "" }; // Not found
}
