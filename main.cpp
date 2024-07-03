#include <iostream>
#include <signal.h>

#include "Listener.h"

Listener* g_listener = nullptr;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Stopping server..." << std::endl;
    if (g_listener != nullptr) {
        g_listener->stopListener();
    }
    exit(signum);
}

int main() {
    std::string ip_address = "127.0.0.1"; // 监听的IP地址
    int port = 8080; // 监听的端口号

    Listener listener(ip_address, port); // 创建Listener实例
    g_listener = &listener;

    // 启动监听器
    ListenerSetupErrorCode startCode = listener.startListener();
    if (startCode != ListenerSetupErrorCode_Success) {
        std::cerr << "Failed to start listener, error code: " << startCode << std::endl;
        return 1;
    }

    std::cout << "Server is listening on IP " << ip_address << " and port " << port << std::endl;

    // 设置信号处理函数，以便在接收到中断信号时停止服务器
    signal(SIGINT, signalHandler);

    // 简单的方式保持主线程运行
    while (true) {
        sleep(1); // 保持主线程活动，可以使用更优雅的方式保持长时间运行。
    }

    return 0;
}