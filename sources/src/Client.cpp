//
// Created by colllijo on 10.04.2023.
//

#include "../header/Client.hpp"

Client::Client(Socket* serverSocket, int bufferSize): m_running(true), m_bufferSize(bufferSize), m_buffer(new char[bufferSize])
{
    m_socket = new Socket();
    if(!m_socket->Accept(serverSocket))
    {
        delete this;
    }

    m_thread = std::thread(&Client::HandleClient, this);
}

void Client::HandleClient()
{
    while (m_running)
    {
        int bytesReceived = m_socket->Receive(m_buffer, m_bufferSize);
        if(bytesReceived == -1)
        {
            std::cerr << "Error receiving message" << std::endl;
            break;
        }
        if(bytesReceived == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::cout << "Received: " << std::string(m_buffer, 0, bytesReceived) << std::endl;

        // Default response: ping
        m_socket->Send(m_buffer, bytesReceived + 1);
    }
}

Client::~Client()
{
    m_running = false;
    m_thread.join();

    m_socket->Close();
    delete m_socket;

    m_bufferSize = 0;
    delete m_buffer;
}