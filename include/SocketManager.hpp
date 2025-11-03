#ifndef __SOCKET_MANAGER_HPP_
#define __SOCKET_MANAGER_HPP_

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ConfigManager.hpp"
#include <unordered_map>

class SocketManager {
public:
    void setup_sockets(const ConfigManager& config, const std::string& myId);
    int get_fd_by_role(const std::string& role) const;

private:
    int create_socket();
    void set_nonblocking(int fd);
    int create_and_bind_socket(const SocketConfig& config, sockaddr_in& addr);

    std::unordered_map<std::string, int> roleFdMap;
};

#endif
