#ifndef EPOLL_MANAGER_HPP
#define EPOLL_MANAGER_HPP

#include <vector>
#include <cstdint>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include "PacketHandler.hpp"

class EpollManager {
private:
    int epoll_fd;

    static constexpr int MAX_EVENTS = 10;
    static constexpr int MAXLINE = 1024;

public:
    EpollManager();
    ~EpollManager();

    void addFd(int fd);
    void waitAndHandle(PacketHandler& handler);
};

#endif
