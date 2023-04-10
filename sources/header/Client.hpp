//
// Created by colllijo on 10.04.2023.
//

#ifndef CC_SERVER_CLIENT_HPP
#define CC_SERVER_CLIENT_HPP

#include <thread>

#include "Socket.hpp"

class Client {
public:
    bool m_running;

    Client(Socket* serverSocket, int bufferSize = 4096);
    ~Client();
private:
    Socket* m_socket;

    int m_bufferSize;
    char* m_buffer;

    std::thread m_thread;
    void HandleClient();
};


#endif //CC_SERVER_CLIENT_HPP
