//
// Created by colllijo on 09.04.2023.
//

#include "../header/Server.hpp"

bool Server::Init(const char* bindingIp, int port, int bufferSize)
{
    m_bindingIp = bindingIp;
    m_port = port;

    m_listeningSocket = new Socket();
    if(!m_listeningSocket->Bind(m_bindingIp, m_port) || !m_listeningSocket->Listen())
    {
        return false;
    }

    m_bufferSize = bufferSize;
    m_buffer = new char[m_bufferSize];

    return true;
}

bool Server::AcceptConnection()
{
    Client* client = new Client(m_listeningSocket);
    m_clients.push_back(client);

    return true;
}

void Server::Close()
{
    delete m_bindingIp;
    m_port = 0;

    m_listeningSocket->Close();
    delete m_listeningSocket;

    for (auto & m_client : m_clients)
    {
        delete m_client;
    }
    std::destroy(m_clients.begin(), m_clients.end());

    m_bufferSize = 0;
    delete m_buffer;
}