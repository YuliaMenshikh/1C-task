#include <iostream>
#include "Client.h"
#include "Server.h"

int main() {
    Server server(8000);

    Client client1(8000);
    Client client2(8000);

    server.AcceptClients();

    client1.WriteMessage("lalalal");
    client2.WriteMessage("popopop");
    std::vector<std::string> res = client1.RequestMyInfo();
    for (auto i : res) {
        std::cout << i << std::endl;
    }

    return 0;
}