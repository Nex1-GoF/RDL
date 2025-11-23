#include "ConfigManager.hpp"
#include "SocketManager.hpp"
#include "EpollManager.hpp"
#include "PacketHandler.hpp"
#include <iostream>
#include <unordered_map>

int main() {
    const char* myId = "D001";  // 현재 시스템 ID
    // 리눅스
    const char* configPath = "/home/user/Project/RDL/config_local.txt";
    // 페타리눅스
    // const char* configPath = "/home/petalinux/config.txt";

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
        if (std::strncmp(role.c_str(), "tx", 2) != 0) {  // 수신용 소켓만 epoll에 등록
            int fd = socketManager.get_fd_by_role(role.c_str());
            if (fd != -1) {
                epollManager.addFd(fd);
            }
        }
    }

    // 4. fdRoleMap 구성
    std::unordered_map<int, const char*> fdRoleMap;
    for (const auto& [role, cfg] : cfgMap) {
        int fd = socketManager.get_fd_by_role(role.c_str());
        if (fd != -1) {
            fdRoleMap[fd] = cfg.role;
        }
    }

    // 5. PacketHandler 생성
    int tx_fd = socketManager.get_fd_by_role("tx");
    PacketHandler handler(fdRoleMap, config, tx_fd);

    // 6. 이벤트 루프 시작
    epollManager.waitAndHandle(handler);

    std::cout << "[main] Shutdown complete\n";
    return 0;
}
