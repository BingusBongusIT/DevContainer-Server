//
// Created by colllijo on 10.04.2023.
//

#include "../header/Client.hpp"

Client::Client(Socket* serverSocket, int bufferSize): m_running(true), m_loggedIn(false), m_chatConnected(false), m_chatAccepted(false), m_chatPartner(nullptr)
{
	uuid_generate(m_id);

    m_socket = new Socket();
    if(!m_socket->Accept(serverSocket))
    {
        delete this;
    }
}

int Client::SendMessage(void *buffer, int bufferSize)
{
    return m_socket->Send(buffer, bufferSize);
}

int Client::ReceiveMessage(void *buffer, int bufferSize)
{
	return m_socket->Receive(buffer, bufferSize);
}

void Client::SetChatPartner(Client *partner)
{
	m_chatPartner = partner;
}

Client* Client::GetChatPartner()
{
	return m_chatPartner;
}

int Client::SendChatMessage(void *buffer, int bufferSize)
{
	return m_chatPartner->SendMessage(buffer, bufferSize);
}

Client::~Client()
{
    m_running = false;

	uuid_clear(m_id);

	m_socket->Close();
    delete m_socket;
}
