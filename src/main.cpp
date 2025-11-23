#include "ConfigManager.hpp"
#include "SocketManager.hpp"
#include "EpollManager.hpp"
#include "PacketHandler.hpp"
#include <iostream>
#include <unordered_map>

// #include "SecurityHandler.hpp"
// #include "packet/HeaderPacket.hpp"
// #include "packet/TgtInfoPacket.hpp"

// auto dumpHex = [](const char* title, const std::vector<uint8_t>& data) {
//     std::cout << title << " (" << data.size() << " bytes): ";
//     for (size_t i = 13; i < data.size(); ++i) {
//         printf("%02X ", data[i]);
//     }
//     std::cout << std::endl;
// };

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

    // // === 보안 테스트 ===
    // std::array<uint8_t, 32> key = {
    //     0x3A, 0xF1, 0x92, 0x7C,
    //     0xB8, 0x44, 0x0E, 0xD5,
    //     0x6F, 0x23, 0xAA, 0x09,
    //     0xCD, 0x55, 0x81, 0x1F,
    //     0x74, 0xC0, 0x39, 0xE8,
    //     0x12, 0x67, 0x4A, 0xB2,
    //     0x8F, 0x90, 0x3D, 0xE4,
    //     0x56, 0x7B, 0xA1, 0xCC
    // };

    // HeaderPacket hdr("C001", "M001", 100, 24);
    // TgtInfoPacket tgtInfo(hdr, 1, 1, 1, 2, 2, 2, 50);

    // std::vector<uint8_t> plain = tgtInfo.serialize();

    // std::vector<uint8_t> encrypted = SecurityHandler::encryptPayload(plain, key);
    // std::vector<uint8_t> decrypted = SecurityHandler::decryptPayload(encrypted, key);

    // if (plain == decrypted) {
    //     std::cout << "[SECURITY TEST] OK: decrypted == original" << std::endl;
    // } else {
    //     std::cout << "[SECURITY TEST] FAIL: decrypted != original" << std::endl;
    // }

    // dumpHex("[PLAIN]", plain);
    // dumpHex("[ENCRYPTED]", encrypted);
    // dumpHex("[DECRYPTED]", decrypted);

    std::cout << "[main] Shutdown complete\n";

    return 0;
}



