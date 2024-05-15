#include "Server.hpp"

Server::Server()
{
}

Server::Server(char *port, char *pwd)
{
	this->port = std::chartoInt(port);
	this->pwd = pwd;
}

Server::create()
{
	socket.bind(this->port);
	socket.listen();
}

Server::run()
{
	this->kq = kqueue();
	if (this->kq == -1)
		throw	std::runtime_error("kqueue() failed");
}
