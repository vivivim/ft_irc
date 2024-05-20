#include "Client.hpp"

Client::Client()
{
}

Client::Client(int socket, std::string msg) : fd(socket), msg(msg), nick("new_"), isPass(false)
{
}

Client::~Client()
{
}

void	Client::setMsg(std::string msg) { this->msg = msg; }

void	Client::attachMsg(std::string msg) { this->msg += msg; }
void	Client::setIsPass(bool TF) { this->isPass = TF; }
void	Client::setIsUserName(bool TF) { this->isUsername = TF; }
void	Client::setIsNick(bool TF)  { this->isNick = TF; }
void	Client::setUserName(std::string input) { this->username = input; }
void	Client::setNick(std::string nick)
{
	this->nick = nick;
}

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }
std::string Client::getNick() { return nick; }
bool		Client::getIsPass() { return isPass; }
bool		Client::getIsUsername() { return isUsername; }
