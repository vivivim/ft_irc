#include "Client.hpp"

Client::Client()
{
}

Client::Client(int socket, std::string msg) : fd(socket), msg(msg), nick("new_"), isPass(false), isUsername(false), isNick(false)
{
}

Client::~Client()
{
}

void	Client::setMsg(std::string msg) { this->msg = msg; }

void	Client::attachMsg(std::string msg) { this->msg += msg; }
void	Client::setIsPass(bool TF) { this->isPass = TF; }
void	Client::setIsUsername(bool TF) { this->isUsername = TF; }
void	Client::setIsNick(bool TF)  { this->isNick = TF; }
void	Client::setUsername(std::string input) { this->username = input; }
void	Client::setNick(std::string nick)
{
	this->nick = nick;
}

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }
bool		Client::getIsPass() { return isPass; }
bool		Client::getIsUsername() { return isUsername; }
std::string Client::getNick() { return nick; }
