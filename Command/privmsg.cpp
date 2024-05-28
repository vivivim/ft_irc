#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include <iostream>
void	Server::privmsg(std::stringstream& ss)
{
	std::string	channel;
	std::string	msg;
	if (!(ss >> channel))
	{
		//무슨 에러?
		return ;
	}
	if (!(ss >> msg))
	{
		//이런 일이 있을 수 있나?
		return ;
	}
	sendMsgToChannel(channel, msg);
}
