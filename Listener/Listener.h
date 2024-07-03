#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <atomic>

#include "Responder.h"

enum ListenerSetupErrorCode {
    ListenerSetupErrorCode_Success = 0,
    ListenerSetupErrorCode_CreateSocketFailed = 1,
    ListenerSetupErrorCode_SetSockOptFailed = 2,
    ListenerSetupErrorCode_BindAddressFailed = 3,
    ListenerSetupErrorCode_ListenFailed = 4,
    ListenerSetupErrorCode_AcceptFailed = 5,
};

const int kMaxWaitInLineClientCount = 1024;

class Listener
{
private:
    int server_fd;
    int port;
    std::string ip_address;
    struct sockaddr_in address;
    Responder responder;

    std::thread listener_thread;
    std::atomic<bool> running;

    ListenerSetupErrorCode listenForConnections();
public:
    Listener(const std::string& ip, int port);
    ~Listener();

    ListenerSetupErrorCode setupListener();
    ListenerSetupErrorCode startListener();
    void stopListener();
};
