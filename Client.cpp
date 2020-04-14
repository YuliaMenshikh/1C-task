#include "Client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>


Client::Client(int port)
{
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = inet_addr("127.0.0.1")
    };
    connect(_socket_fd, (struct sockaddr*)& addr, sizeof(addr));
}

void Client::WriteMessage(const char* message)
{
    write(_socket_fd, message, strlen(message));
}

std::vector< std::string > Client::RequestInfo(int client_id) const
{
    char buffer[4096];
    sprintf(buffer, "request %d", client_id);
    write(_socket_fd, buffer, sizeof(buffer));
    std::vector<std::string> result;
    while (read(_socket_fd, buffer, sizeof(buffer)) > 0) {
        result.emplace_back(buffer);
    }

    return result;
}

std::vector< std::string > Client::RequestMyInfo() const
{
    const char* message = "request my";
    write(_socket_fd, message, sizeof(message));
    std::vector<std::string> result;
    char buffer[4096];
    while (read(_socket_fd, buffer, sizeof(buffer)) > 0) {
        result.emplace_back(buffer);
    }
    return result;
}


