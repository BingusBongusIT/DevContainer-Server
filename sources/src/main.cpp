#include <iostream>

#include "../header/Server.hpp"

int main(int argumentCount, char* arguments[])
{
    const char* bindingIp = "0.0.0.0";
    int port = 42024;

    Server server;
    if(!server.Init(bindingIp, port))
    {
        std::cerr << "Error initializing the server." << std::endl;
        return 1;
    }
    std::cout << "Server ready for connections." << std::endl;

    while(true)
    {
        std::cout << "Waiting for client to connect" << std::endl;
        if(!server.AcceptConnection())
        {
            std::cerr << "Error accepting incoming connection." << std::endl;
            return 1;
        }
        std::cout << "Accepted new client" << std::endl;
    }

    server.Close();

    return 0;
}