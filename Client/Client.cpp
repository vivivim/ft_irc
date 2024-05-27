#include "Client.hpp"

Client::Client()
{
}

Client::Client(int socket, std::string msg) : fd(socket), msg(msg), prefix(""), nick("new_"), isPass(false), isUsername(false), isNick(false), isConnected(false)
{
}

Client::~Client()
{
}

void	Client::setMsg(std::string msg) { msg = msg; }
void	Client::attachMsg(std::string msg) { msg += msg; }
void	Client::setIsPass(bool TF) { isPass = TF; }
void	Client::setIsUsername(bool TF) { isUsername = TF; }
void	Client::setOperator() { prefix = "@"; }
void	Client::unsetOperator() { prefix = ""; }
void	Client::setIsNick(bool TF)  { isNick = TF; }
void	Client::setUsername(std::string input) { username = input; }
void	Client::setNick(std::string nick) { nick = nick; }
void	Client::setIsConnected(bool TF) { this->isConnected = TF; }

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }
bool		Client::getIsPass() { return isPass; }
bool		Client::getIsUsername() { return isUsername; }
std::string	Client::getPrefix() { return prefix; }
std::string Client::getNick() { return nick; }
bool		Client::getIsNick() { return isNick; }
bool		Client::getIsConnected() { return isConnected; }
