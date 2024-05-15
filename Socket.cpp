#include "Socket.hpp"

Socket::Socket()
{
	this->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socket == -1)
		throw	std::runtime_error("Could not create socket");
}

Socket::~Socket()
{
}

Socket::bind(int port)
{
	sockaddr_in	serverAddress;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	if (::bind(this->socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw	std::runtime_error("Bind failed");
}

Socket::listen()
{
	if (listen(this->socket, 5) < 0)
		throw	std::runtime_error("Listen failed");
}
