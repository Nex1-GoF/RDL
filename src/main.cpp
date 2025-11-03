#include "ConfigManager.hpp"
#include "SocketManager.hpp"
#include "EpollManager.hpp"
#include "PacketHandler.hpp"
#include <iostream>

int main() {
    const char* myId = "D001";  // 현재 시스템 ID
    const char* configPath = "/home/user/Project/RDL/config_local.txt";

    // 1. 설정 로드
    ConfigManager config;
    if (!config.load(configPath)) {
        std::cerr << "[main] Failed to load config file\n";
        return 1;
    }

    // 2. 소켓 바인딩
    SocketManager socketManager;
    socketManager.setup(config, myId);

    // 3. epoll 등록
    EpollManager epollManager;
    auto cfgMap = config.getConfigMapById(myId);
    for (const auto& [role, cfg] : cfgMap) {
        if (role != "tx") {  // 수신용 소켓만 epoll에 등록
            int fd = socketManager.get_fd_by_role(role);
            epollManager.addFd(fd);
        }
    }

    // 4. PacketHandler 생성
    std::unordered_map<int, std::string> fdRoleMap;
    for (const auto& [role, cfg] : cfgMap) {
        int fd = socketManager.get_fd_by_role(role);
        fdRoleMap[fd] = role;
    }

    int tx_fd = socketManager.get_fd_by_role("tx");
    PacketHandler handler(fdRoleMap, config, tx_fd);

    // 5. 이벤트 루프 시작
    epollManager.waitAndHandle(handler);

    std::cout << "[main] Shutdown complete\n";
    return 0;
}
