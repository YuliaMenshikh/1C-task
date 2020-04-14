#include "Server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <cstdlib>

Server::Server(int port) : _new_client_id(0)
{
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = inet_addr("127.0.0.1")
    };
    if (bind(_socket_fd, (struct sockaddr *)&_socket_fd, sizeof(addr)) == -1) {

    }
    if (listen(_socket_fd, SOMAXCONN) == -1) {

    }
}

void Server::AcceptClients() const
{
    while (true) {
        int client_fd = accept(_socket_fd, nullptr, nullptr);
        std::thread client_processing(&Server::ProcessClient, this,  client_fd);
        client_processing.detach();
    }
}

void Server::ProcessClient(int client_fd)
{
    uint32_t client_id = _new_client_id.fetch_add(1);

    const size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    //read request from the client
    while (read(client_fd, buffer, BUF_SIZE) > 0) {
        int request_client_id = -1;
        request_type request = GetRequestType(buffer, &request_client_id);
        switch (request) {
            case request_type::post :
                SaveMessage(client_id, buffer);
                break;
            case request_type::get_other :
                GetClientInfo(request_client_id, client_fd);
                break;
            case request_type::get_self :
                GetClientInfo(client_id, client_fd);
                break;
            default:
                break;
        }
    }

}

request_type Server::GetRequestType(char *mes, int *id)
{
    char buffer[4096];
    sscanf(mes, "%s", buffer);
    if (strcmp("request", buffer) == 0) {
        sscanf(mes, "%s", buffer);
        if (strcmp("my", buffer) == 0)
            return request_type::get_self;
        *id = strtol(buffer, nullptr, 10);
        return request_type::get_other;
    }
    return request_type::post;
}

void Server::SaveMessage(uint32_t client_id, char *mes)
{
    _mutex.lock();
    FILE* file = fopen("file_name.txt", "a");
    fprintf(file, "%d - %s", client_id, mes);
    fclose(file);
    _mutex.unlock();
}

void Server::GetClientInfo(uint32_t client_id, int client_fd)
{
    FILE* file = fopen("file_name.txt", "r");
    int id;
    char buffer[4096];
    while (fscanf(file, "%d - %s", &id, buffer) > 0) {
        if (client_id == id) {
            write(client_fd, buffer, sizeof(buffer));
        }
    }
    fclose(file);
}

