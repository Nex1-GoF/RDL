#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP

#include "ConfigManager.hpp"
#include <unordered_map>
#include <string>

class SocketManager {
public:
    void setup(const ConfigManager& config, const char* myId);
    int get_fd_by_role(const char* role) const;

private:
    int create_socket();
    void set_nonblocking(int fd);
    int create_and_bind(const SocketConfig& cfg);

    std::unordered_map<std::string, int> roleFdMap;
};

#endif