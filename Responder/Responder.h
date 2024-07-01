#pragma once

#include <iostream>

class Responder
{
private:
    /* data */
public:
    Responder(/* args */);
    ~Responder();

    void handleClient(int client_socket);
};
