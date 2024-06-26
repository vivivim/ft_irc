#include "Socket.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>

Socket::Socket()
{
}

Socket::~Socket()
{
	close(socket);
}

int		Socket::getSocket() { return socket; }

void	Socket::create()
{
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0)
		throw	std::runtime_error("Could not create socket");
	int opt = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
		throw	std::runtime_error("Could not set socket option");
}

void	Socket::bind(int port)
{
	sockaddr_in	serverAddress;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port);
	if (::bind(socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw	std::runtime_error("Bind failed");
}

void	Socket::listen()
{
	if (::listen(socket, 5) < 0)
		throw	std::runtime_error("Listen failed");
}
