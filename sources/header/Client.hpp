//
// Created by colllijo on 10.04.2023.
//

#ifndef CC_SERVER_CLIENT_HPP
#define CC_SERVER_CLIENT_HPP

#include <thread>
#include <uuid/uuid.h>

#include "Socket.hpp"

class Client {
public:
	uuid_t m_id{};
	std::thread m_thread;

    bool m_running;

    explicit Client(Socket* serverSocket, int bufferSize = 4096);
    ~Client();

    int SendMessage(void* buffer, int bufferSize);
    int ReceiveMessage(void* buffer, int bufferSize);
private:
    Socket* m_socket;
};


#endif //CC_SERVER_CLIENT_HPP
