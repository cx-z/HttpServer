#include "Listener.h"

#include <signal.h>

// 使用全局变量listener
Listener* g_listener = nullptr;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Stopping server..." << std::endl;
    if (g_listener != nullptr) {
        g_listener->stopListener();
    }
    exit(signum);
}

int main() {
    Listener listener("127.0.0.1", 8080);
    g_listener = &listener;

    // 启动监听器
    ListenerSetupErrorCode startCode = listener.startListener();
    if (startCode != ListenerSetupErrorCode_Success) {
        std::cerr << "Failed to start listener, error code: " << startCode << std::endl;
        return 1;
    }

    std::cout << "Server is listening on 127.0.0.1:8080 " << std::endl;

    // 简单的方式保持主线程运行，接收信号以停止服务器
    signal(SIGINT, signalHandler);

    while(true) {
        sleep(1); // 保持主线程活动，可以使用更优雅的方式保持长时间运行。
    }

    return 0;
}