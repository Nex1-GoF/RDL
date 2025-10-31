#include "EpollManager.hpp"
#include "packet/HeaderPacket.hpp"
#include <iostream>

using namespace std;

void EpollManager::addFd(int fd)
{
    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
    {
        perror("epoll_ctl failed");
        exit(EXIT_FAILURE);
    }
    cout << "fd added to epoll" << "\n";
}

void EpollManager::waitAndHandle(const char *hello)
{
    epoll_event events[MAX_EVENTS];
    char buffer[MAXLINE];
    sockaddr_in clientAddr{};
    socklen_t len = sizeof(clientAddr);

    while (true)
    {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n < 0 && errno != EINTR)
        {
            perror("epoll_wait error");
            break;
        }

        for (int i = 0; i < n; ++i)
        {
            int curfd = events[i].data.fd;
            if (events[i].events & EPOLLIN)
            {
                while (true)
                {
                    int recvsize = recvfrom(curfd, buffer, MAXLINE, 0,
                                            (sockaddr *)&clientAddr, &len);
                    if (recvsize > 0)
                    {
                        HeaderPacket header;
                        header = header.deserialize((uint8_t *)buffer);
                        header.print();

                        // buffer[recvsize] = '\0';
                        // printf("Client: %s\n", buffer);

                        HeaderPacket responseHeader('D', '2',
                             header.getDestType(), header.getDestId(), header.getSeq(), 9);
                        
                        uint8_t data[9];
                        responseHeader.serialize(data);
                        

                        sendto(curfd, data, sizeof(data), 0,
                               (const sockaddr *)&clientAddr, len);
                        std::cout << "Hello message sent." << std::endl;

                        if (strcmp(buffer, "end") == 0)
                            return;
                    }
                    else if (recvsize == -1 &&
                             (errno == EAGAIN || errno == EWOULDBLOCK))
                    {
                        break;
                    }
                    else
                    {
                        perror("recvfrom");
                        break;
                    }
                }
            }
        }
    }
}
