#include "Listener.h"

Listener::Listener(/* args */)
{
    responder = Responder();
}

Listener::~Listener()
{
    stopListener();
    close(sock);
    close(server_fd);
}

ListenerSetupErrorCode Listener::setupListener() {
    // 创建 socket 文件描述符
    int createSockCode = (server_fd = socket(AF_INET, SOCK_STREAM, 0));
    if (createSockCode == 0) {
        perror("socket failed");
        return ListenerSetupErrorCode_CreateSocketFailed;
    }

    // 强制端口和地址的使用
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return ListenerSetupErrorCode_SetSockOptFailed;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
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