#pragma once

#include <vector>
#include <string>

class Client
{
public:
    Client(int port);
    void WriteMessage(const char* messge);
    std::vector< std::string > RequestInfo(int client_id) const;
    std::vector< std::string > RequestMyInfo() const;
private:
    int _socket_fd;
};