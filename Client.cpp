#include "Client.hpp"

Client::Client()
{
}

Client::Client(int socket, std::string msg) : fd(socket), msg(msg), nick("new_")
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

void	Client::setNick(std::string nick)
{
	this->nick = nick;
}

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }
std::string Client::getNick() { return nick; }
