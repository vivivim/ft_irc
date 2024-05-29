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
	while (ss >> msg) //이거 실패할 수도 있나?
	{
		tempMsg += msg + " ";
		std::cout << tempMsg;
	}
	// :user2!root@127.0.0.1 PRIVMSG #chan :hi
	msg = ":" + currClient.getNick() + ADR + " PRIVMSG " + channel + " " + tempMsg;
	sendMsgToChannelExceptMe(channel, msg, currClient);
}
