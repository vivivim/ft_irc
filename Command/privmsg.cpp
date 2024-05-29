#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include <iostream>

void	Server::privmsg(std::stringstream& ss, Client currClient)
{
	std::string	channel;
	std::string	msg;
	if (!(ss >> channel))
	{
		//무슨 에러?
		return ;
	}
	std::string	tempMsg;
	while (ss >> msg)
	{
		tempMsg += msg + " ";
		//이런 일이 있을 수 있나?
		std::cout << tempMsg;
		return ;
	}
	// :user2!root@127.0.0.1 PRIVMSG #chan :hi
	msg = ":" + currClient.getNick() + ADR + " PRIVMSG " + channel + " " + tempMsg;
	sendMsgToChannelExceptMe(channel, msg, currClient);
}
