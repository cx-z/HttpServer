#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>

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
    struct sockaddr_in address;
    int sock;
    Responder responder;

    std::thread listener_thread;
    std::atomic<bool> running;

    ListenerSetupErrorCode setupListener();
    ListenerSetupErrorCode startListener();
    void stopListener();
    ListenerSetupErrorCode listenForConnections();
public:
    Listener(/* args */);
    ~Listener();
};
