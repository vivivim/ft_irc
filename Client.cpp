#include "Client.hpp"

Client::Client()
{
}

Client::Client(int socket, std::string msg) : fd(socket), msg(msg), isPass(false)
{
}

Client::~Client()
{
}

void	Client::setMsg(std::string msg)
{
	this->msg = msg;
}

void	Client::attachMsg(std::string msg)
{
	this->msg += msg;
}

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }

