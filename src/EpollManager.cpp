#include "EpollManager.hpp"
#include "packet/HeaderPacket.hpp"
#include <iostream>
#include <vector>
#include <cstring>  // for std::strcmp
#include <unistd.h> // for close()

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
    std::cout << "fd added to epoll" << std::endl;
}

void EpollManager::waitAndHandle(const char* hello)
{
    epoll_event events[MAX_EVENTS];
    uint8_t buffer[MAXLINE];
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
                                            (sockaddr*)&clientAddr, &len);
                    if (recvsize > 0)
                    {
                        if (recvsize < HeaderPacket::SIZE)
                        {
                            std::cerr << "Received data too small for HeaderPacket" << std::endl;
                            continue;
                        }

                        std::vector<uint8_t> packetData(buffer, buffer + recvsize);

                        try {
                            HeaderPacket header = HeaderPacket::deserialize(packetData);
                            header.print();

                            HeaderPacket responseHeader(
                                'D', '2',
                                header.getDestType(),
                                header.getDestId(),
                                header.getSeq(),
                                9
                            );

                            std::vector<uint8_t> responseData = responseHeader.serialize();

                            sendto(curfd, responseData.data(), responseData.size(), 0,
                                   (const sockaddr*)&clientAddr, len);
                            std::cout << "Hello message sent." << std::endl;

                            if (recvsize >= 3 && std::memcmp(buffer, "end", 3) == 0)
                                return;

                        } catch (const std::exception& e) {
                            std::cerr << "Deserialization error: " << e.what() << std::endl;
                        }
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
