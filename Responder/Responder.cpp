#include "Responder.h"

#include <unistd.h>

Responder::Responder(/* args */)
{
}

Responder::~Responder()
{
}

void Responder::handleClient(int client_socket) {
    char buffer[30000] = {0};
    long valread = read(client_socket, buffer, 30000);
    std::cout << buffer << std::endl;

    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world";
    write(client_socket, hello, strlen(hello));
    std::cout << "Hello message sent\n";

    close(client_socket);
}
