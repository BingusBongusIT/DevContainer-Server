//
// Created by colllijo on 09.04.2023.
//

#ifndef CC_SERVER_SERVER_H
#define CC_SERVER_SERVER_H

#include <cstring>
#include <iostream>
#include <vector>
#include <thread>

#include "Socket.hpp"
#include "Client.hpp"

class Server {
public:
	bool m_running;

    bool Init(const char* bindingIp, int port, int bufferSize = 4096);
    void AcceptConnections();
    void Close();

private:
    const char* m_bindingIp;
    int m_port;

	std::thread m_manager;
    Socket* m_listeningSocket;
    std::vector<Client*> m_clients;

    int m_bufferSize;
    char* m_buffer;

	void ManageServer();
	void HandleClient(Client* client);
};


#endif //CC_SERVER_SERVER_H