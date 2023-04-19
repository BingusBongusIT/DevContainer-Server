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
	std::string m_username;
	std::thread m_thread;

    bool m_running;
	bool m_loggedIn;
	bool m_chatAccepted;
	bool m_chatConnected;

    explicit Client(Socket* serverSocket, int bufferSize = 4096);
    ~Client();

	void SetChatPartner(Client* partner);
	Client* GetChatPartner();

    int SendMessage(void* buffer, int bufferSize);
    int ReceiveMessage(void* buffer, int bufferSize);
	int SendChatMessage(void* buffer, int bufferSize);
private:
    Socket* m_socket;

	Client* m_chatPartner;
};


#endif //CC_SERVER_CLIENT_HPP
