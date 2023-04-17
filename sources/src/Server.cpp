//
// Created by colllijo on 09.04.2023.
//

#include "../header/Server.hpp"

bool Server::Init(const char* bindingIp, int port, int bufferSize)
{
	m_running = true;

    m_bindingIp = bindingIp;
    m_port = port;

    m_listeningSocket = new Socket();
    if(!m_listeningSocket->Bind(m_bindingIp, m_port) || !m_listeningSocket->Listen())
    {
        return false;
    }

    m_bufferSize = bufferSize;
    m_buffer = new char[m_bufferSize];

	m_manager = std::thread(&Server::ManageServer, this);
    return true;
}

void Server::ManageServer()
{
	std::thread connections(&Server::AcceptConnections, this);

	while (m_running)
	{
		// Manage the clients
		for(auto client : m_clients)
		{
			if (!client->m_running)
			{
				client->m_thread.join();
				m_clients.erase(std::find(m_clients.begin(), m_clients.end(), client));
				delete client;
			}
		}
	}

	connections.detach();
}

void Server::AcceptConnections()
{
	while (m_running)
	{
		auto* client = new Client(m_listeningSocket);
		client->m_thread = std::thread(&Server::HandleClient, this, client);

		char clientId[36];
		uuid_unparse(client->m_id, clientId);

		// Tell the client it's id

		std::cout << "New client connected with id: " << clientId << std::endl;
		m_clients.push_back(client);
	}
}

void Server::HandleClient(Client *client)
{
	while (client->m_running)
	{
		int bytesReceived = client->ReceiveMessage(m_buffer, m_bufferSize);
		if(bytesReceived == -1)
		{
			std::cerr << "Error receiving message" << std::endl;
			break;
		}
		if(bytesReceived == 0)
		{
			char clientId[36];
			uuid_unparse(client->m_id, clientId);

			std::cout << "Client " << clientId << " disconnected" << std::endl;

			auto clientIterator = std::find(m_clients.begin(), m_clients.end(), client);
			m_clients.erase(clientIterator);
			break;
		}

		std::cout << "Received: " << std::string(m_buffer, 0, bytesReceived) << std::endl;

		// Default response: broadcast
		client->SendMessage(m_buffer, bytesReceived + 1);
	}
}

void Server::Close()
{
	std::cout << "closing" << std::endl;
	m_running = false;
	m_manager.join();

    delete m_bindingIp;

    m_listeningSocket->Close();
    delete m_listeningSocket;

    for (auto &m_client : m_clients)
    {
        delete m_client;
    }
    std::destroy(m_clients.begin(), m_clients.end());

    m_bufferSize = 0;
    delete m_buffer;
}