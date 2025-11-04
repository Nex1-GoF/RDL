#include "SocketManager.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>

int SocketManager::create_socket() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void SocketManager::set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("set non-blocking failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
}

int SocketManager::create_and_bind(const SocketConfig& cfg) {
    int fd = create_socket();
    set_nonblocking(fd);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(cfg.port);
    inet_pton(AF_INET, cfg.ip, &addr.sin_addr);

    if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("bind failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Bound " << cfg.role << " to " << cfg.ip << ":" << cfg.port << std::endl;
    return fd;
}

void SocketManager::setup(const ConfigManager& config, const char* myId) {
    auto cfgMap = config.getConfigMapById(myId);
    for (const auto& [role, cfg] : cfgMap) {
        int fd = create_and_bind(cfg);
        roleFdMap[role] = fd;
    }
}

int SocketManager::get_fd_by_role(const char* role) const {
    auto it = roleFdMap.find(std::string(role));
    return (it != roleFdMap.end()) ? it->second : -1;
}
