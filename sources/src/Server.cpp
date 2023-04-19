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

		std::string input = std::string(m_buffer, 0, bytesReceived);
		std::string type;

		std::cout << "Received: " << input.c_str() << std::endl;

		if (client->m_chatAccepted)
		{
			if(input == "msg: !accept")
			{
				for(auto _client: m_clients)
				{
					char id[36];
					if(_client == client->GetChatPartner())
					{

						client->m_chatConnected = true;
						_client->m_chatConnected = true;
						_client->SetChatPartner(client);

						std::string chatMessage;

						uuid_unparse(client->GetChatPartner()->m_id, id);
						chatMessage = " --- started a chat with ";
						chatMessage += id;
						chatMessage += " --- ";
						client->SendMessage((void *) chatMessage.c_str(), chatMessage.size() + 1);

						uuid_unparse(_client->GetChatPartner()->m_id, id);
						chatMessage = " --- started a chat with ";
						chatMessage += id;
						chatMessage += " --- ";
						_client->SendMessage((void *) chatMessage.c_str(), chatMessage.size() + 1);
					}
				}
			}
			else
			{
				client->SetChatPartner(nullptr);
			}
			client->m_chatAccepted = false;
			continue;
		}

		std::string::size_type separator = input.find(':', 0);
		if(separator != std::string::npos)
		{
			type =  input.substr(0, separator);

			if(type == "command")
			{
				// switch for the four commands
				std::string command = input.substr(separator + 2);
				std::string arguments;

				//Check if command has arguments
				std::string::size_type endOfCommand = command.find('|', 0);
				if (endOfCommand != std::string::npos)
				{
					arguments = command.substr(endOfCommand + 1);
					command = command.substr(0, endOfCommand);
				}

				// Check if command exists
				auto commandIterator = std::find(m_commands.begin(), m_commands.end(), command);
				if (commandIterator != m_commands.end())
				{
					// Turn the command into it's index for use in the switch
					int commandIndex = commandIterator - m_commands.begin();
					switch (commandIndex) {
						case 0: // /login
							Login(client, &arguments);
							break;
						case 1: // /logout
							Logout(client);
							break;
						case 2: // /register
							Register(client, &arguments);
							break;
						case 3: // /msg
							StartChat(client, &arguments);
							break;
					}
				}
			}
			else
			{
				if (client->m_chatConnected)
				{
					client->SendChatMessage((void *) input.substr(separator + 1).c_str(), input.substr(separator + 1).size() + 1);
				}
				else
				{
					// Send warning
					std::string warning = "You are currently in no chat. Please join one before sending messages";
					client->SendMessage((void *) warning.c_str(), warning.size() + 1);
				}
			}
		}
	}
}

void Server::Login(Client* client, std::string* arguments)
{

	// TODO: DB STUFF
	// Check if username exists
	// If yes check if password match
	// If no send error messages
	// Set the username
}

void Server::Logout(Client *client)
{
	// logout the client
}

void Server::Register(Client *client, std::string* arguments)
{

	// TODO: DB STUFF
	// Check if username doesn't exist
	// If yes create database entry
	// If no send error messages
	// Set the username
}

void Server::StartChat(Client *client, std::string* arguments)
{
	std::string partner = *arguments;

	//temp
	uuid_t partnerId;
	uuid_parse(partner.c_str(), partnerId);
	char userid[36];
	uuid_unparse(client->m_id, userid);

	for(auto _client : m_clients)
	{
		if(uuid_compare(client->m_id, _client->m_id) == 0)
		{
			continue;
		}

//		if(_client->username == partner)
		if(uuid_compare(_client->m_id, partnerId) == 0)
		{
			// Remove previous partner
			if(client->m_chatConnected)
			{
				client->GetChatPartner()->m_chatConnected = false;
				client->GetChatPartner()->SetChatPartner(nullptr);
				client->m_chatConnected = false;
				client->SetChatPartner(nullptr);
			}

			std::string message = userid;
			message += " would like to start a chat with you: use !accpet to start the chat";

			_client->SendMessage((void *) message.c_str(), message.size() + 1);

			_client->m_chatAccepted = true;
			_client->SetChatPartner(client);
		}
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