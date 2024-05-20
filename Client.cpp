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

void	Client::setMsg(std::string msg) { this->msg = msg; }

void	Client::attachMsg(std::string msg) { this->msg += msg; }
void	Client::setIsPass(bool TF) { this->isPass = TF; }
void	Client::setIsUserName(bool TF) { this->isUsername = TF; }
void	Client::setUserName(std::string input) { this->username = input; }

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }
bool		Client::getIsPass() { return isPass; }
bool		Client::getIsUsername() { return isUsername; }
