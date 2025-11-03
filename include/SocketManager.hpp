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

class SocketManager {
private:
    int tx_fd;         // 송신용
    int msl_info_fd;   // 미사일 정보 수신용
    int msl_com_fd;    // 미사일 명령 수신용
    int tgt_info_fd;   // 표적 정보 수신용
    int src_fd;        // 예비용

    sockaddr_in tx_addr{};
    sockaddr_in msl_info_addr{};
    sockaddr_in msl_com_addr{};
    sockaddr_in tgt_info_addr{};
    sockaddr_in src_addr{};

    // 범용 소켓 생성 및 바인딩 함수
    int create_and_bind_socket(const SocketConfig& config, sockaddr_in& addr);
    int create_socket();
    void set_nonblocking(int fd);
    void bind_socket(int fd, sockaddr_in& addr, uint16_t port);

public:
    SocketManager() = default;
    ~SocketManager() {
        close(tx_fd);
        close(msl_info_fd);
        close(msl_com_fd);
        close(tgt_info_fd);
        close(src_fd);
    }

    void setup_sockets(const ConfigManager& config);

    int get_tx_fd() const        { return tx_fd; }
    int get_msl_info_fd() const  { return msl_info_fd; }
    int get_msl_com_fd() const   { return msl_com_fd; }
    int get_tgt_info_fd() const  { return tgt_info_fd; }
    int get_src_fd() const       { return src_fd; }

    const sockaddr_in& smslock() const        { return tx_addr; }
    const sockaddr_in& get_msl_info_addr() const  { return msl_info_addr; }
    const sockaddr_in& get_msl_com_addr() const   { return msl_com_addr; }
    const sockaddr_in& get_tgt_info_addr() const  { return tgt_info_addr; }
    const sockaddr_in& get_src_addr() const       { return src_addr; }
};

#endif
