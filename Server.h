#pragma once

#include <atomic>
#include <inttypes.h>
#include "mutex"

enum request_type {
    post,
    get_self,
    get_other};

class Server
{
public:
    explicit Server(int port);
    void AcceptClients() const;
private:
    int _socket_fd;
    std::atomic<uint32_t> _new_client_id;
    std::mutex _mutex;
private:
    void ProcessClient(int client_fd);
    request_type GetRequestType(char* mes, int* id);

    void SaveMessage(uint32_t client_id, char* mes);
    void GetClientInfo(uint32_t client_id, int client_fd);
};