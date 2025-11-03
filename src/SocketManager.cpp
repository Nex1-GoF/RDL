#include "SocketManager.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

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

int SocketManager::create_and_bind_socket(const SocketConfig& config, sockaddr_in& addr) {
    int fd = create_socket();
    set_nonblocking(fd);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(config.port);
    inet_pton(AF_INET, config.ip.c_str(), &addr.sin_addr);

    if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("bind failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    return fd;
}

void SocketManager::setup_sockets(const ConfigManager& config, const std::string& myId) {
    auto configs = config.getConfigsById(myId);
    for (const auto& cfg : configs) {
        sockaddr_in addr{};
        int fd = create_and_bind_socket(cfg, addr);
        roleFdMap[cfg.role] = fd;
        std::cout << cfg.role << " socket bound to " << cfg.ip << ":" << cfg.port << std::endl;
    }
}

int SocketManager::get_fd_by_role(const std::string& role) const {
    auto it = roleFdMap.find(role);
    return (it != roleFdMap.end()) ? it->second : -1;
}
