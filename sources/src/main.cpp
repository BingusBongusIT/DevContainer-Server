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

	std::string input;
    while(server.m_running)
    {
		getline(std::cin, input);

		if(input == "stop")
		{
			server.Close();
		}
    }

    return 0;
}