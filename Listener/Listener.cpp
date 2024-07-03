#include "Listener.h"

#include <arpa/inet.h> 

Listener::Listener(const std::string& ip, int p) : ip_address(ip), port(p)
{
    responder = Responder();
    running = false;

    ListenerSetupErrorCode setupCode = setupListener();
    if (setupCode != ListenerSetupErrorCode_Success) {
        std::cerr << "Failed to set up listener, error code: " << setupCode << std::endl;
        // 处理错误，例如抛出异常或设置某种错误状态
    }
}

Listener::~Listener()
{
    stopListener();
    if (server_fd != -1)
        close(server_fd);
}

ListenerSetupErrorCode Listener::setupListener() {
    // 创建 socket 文件描述符
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return ListenerSetupErrorCode_CreateSocketFailed;
    }

    // 强制端口和地址的使用
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt SO_REUSEADDR failed");
        return ListenerSetupErrorCode_SetSockOptFailed;
    }

    // 强制端口重用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt SO_REUSEPORT failed");
        return ListenerSetupErrorCode_SetSockOptFailed;
    }
    address.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_address.c_str(), &address.sin_addr) <= 0) {
        perror("inet_pton failed");
        return ListenerSetupErrorCode_BindAddressFailed;
    }
    address.sin_port = htons(port);

    // 绑定到端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return ListenerSetupErrorCode_BindAddressFailed;
    }

    return ListenerSetupErrorCode_Success;
}

ListenerSetupErrorCode Listener::startListener() {
    // 监听来自客户端的连接，最大等待队列长度为1024
    int listenCode = listen(server_fd, kMaxWaitInLineClientCount);
    if (listenCode < 0) {
        perror("listen");
        return ListenerSetupErrorCode_ListenFailed;
    }
    // Start listening for connections in a new thread
    running = true;
    std::thread listener(&Listener::listenForConnections, this);

    return ListenerSetupErrorCode_Success;
}

void Listener::stopListener() {
    running = false;
    if (listener_thread.joinable()) {
        listener_thread.join();
    }
    
}

ListenerSetupErrorCode Listener::listenForConnections() {
    std::cout << "Listening for connections on port " << port << std::endl;

    int addrlen = sizeof(address);
    while (running) {
        int client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_socket < 0) {
            perror("accept");
            return ListenerSetupErrorCode_AcceptFailed;
        }
        if (!running) {
            break;
        }
        
        // Handle each client connection in a new thread
        std::thread client_handler(&Responder::handleClient, this->responder, client_socket);
        client_handler.detach(); // Detach the client thread to allow it to run independently
    }
}