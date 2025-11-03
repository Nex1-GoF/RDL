#include "SocketManager.hpp"
#include <iostream>

using namespace std;

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

void SocketManager::bind_socket(int fd, sockaddr_in& addr, uint16_t port) {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("bind failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void SocketManager::setup_sockets(const ConfigManager& config) {
    tx_fd        = create_and_bind_socket(config.getSocketConfig("tx"), tx_addr);
    cout << "TX socket created and bound to port " << config.getSocketConfig("tx").ip 
    << " " << config.getSocketConfig("tx").port << "\n";
    msl_info_fd  = create_and_bind_socket(config.getSocketConfig("msl_info"), msl_info_addr);
    cout << "MSL Info socket created and bound to port " << config.getSocketConfig("msl_info").ip
    << " " << config.getSocketConfig("msl_info").port << "\n";
    msl_com_fd   = create_and_bind_socket(config.getSocketConfig("msl_com"), msl_com_addr);
    cout << "MSL Com socket created and bound to port " << config.getSocketConfig("msl_com").ip
    << " " << config.getSocketConfig("msl_com").port << "\n";    
    tgt_info_fd  = create_and_bind_socket(config.getSocketConfig("tgt_info"), tgt_info_addr);
    cout << "TGT Info socket created and bound to port " << config.getSocketConfig("tgt_info").ip
    << " " << config.getSocketConfig("tgt_info").port << "\n";   
    src_fd       = create_and_bind_socket(config.getSocketConfig("src"), src_addr);
    cout << "SRC socket created and bound to port  " << config.getSocketConfig("src").ip
    << " " << config.getSocketConfig("src").port << "\n";
}