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
	}
	tempMsg.erase(tempMsg.size() - 1);
	// :user2!root@127.0.0.1 PRIVMSG #chan :hi
	msg = ":" + currClient.getNick() + ADR + " PRIVMSG " + channel + " " + tempMsg + "\r\n\r\n";
	sendMsgToChannelExceptMe(channel, msg, currClient);

	std::cout << "tempMsg : " << tempMsg << std::endl;
	if (tempMsg == ":letsGoClimbing();")
	{
		std::cout << "소환!\n";
		joinChannel(clients[getClientFdByNick("bot")], channel);
	}
}
