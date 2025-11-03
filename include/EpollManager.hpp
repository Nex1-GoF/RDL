#ifndef __EPOLLMANAGER_HPP__
#define __EPOLLMANAGER_HPP__

#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

#define MAX_EVENTS 10
#define MAXLINE 1024

using namespace std;

class EpollManager {
private:
    int epoll_fd;
    int tx_fd;

public:
    EpollManager() {
        epoll_fd = epoll_create1(0);
        if (epoll_fd < 0) {
            perror("epoll creation failed");
            exit(EXIT_FAILURE);
        }
        cout << "epoll creation " << "\n";
    }

    void addFd(int fd);

    void waitAndHandle(const char* hello);

    void setTxFd(int fd){tx_fd = fd;}

    ~EpollManager() {
        close(epoll_fd);
        close(tx_fd);
    }
};


#endif