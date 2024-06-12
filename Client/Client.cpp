#include "Client.hpp"

Client::Client()
{
}

Client::Client(int socket, std::string msg, std::string ip) : fd(socket), msg(msg), prefix(""), nick("nickname"), ip(ip), isPass(false), isUsername(false), isNick(false), isConnected(false)
{
}

Client::~Client()
{
}

void	Client::setMsg(std::string msg) { this->msg = msg; }
void	Client::attachMsg(std::string msg) { this->msg += msg; }
void	Client::setIsPass(bool TF) { isPass = TF; }
void	Client::setIsUsername(bool TF) { isUsername = TF; }
void	Client::setOperator() { prefix = "@"; }
void	Client::unsetOperator() { prefix = ""; }
void	Client::setIsNick(bool TF)  { isNick = TF; }
void	Client::setUsername(std::string input) { username = input; }
void	Client::setNick(std::string nick) { this->nick = nick; }
void	Client::setIsConnected(bool TF) { this->isConnected = TF; }

int			Client::getFd() { return fd; }
std::string Client::getMsg() { return msg; }
std::string	Client::getPrefix() { return prefix; }
bool		Client::getIsPass() { return isPass; }
bool		Client::getIsUsername() { return isUsername; }
std::string Client::getNick() { return nick; }
std::string Client::getIPaddr() { return ip; }
bool		Client::getIsNick() { return isNick; }
bool		Client::getIsConnected() { return isConnected; }
