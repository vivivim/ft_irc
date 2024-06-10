#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

#include <iostream>

void	Server::privmsg(std::stringstream& ss, Client currClient)
{
	std::string	channel;
	if (!(ss >> channel))
	{
		//무슨 에러?
		return ;
	}
	std::string comment;
	if (ss >> comment)
	{
		if (comment[0] == ':')
		{
			std::string tmp;
			while (ss >> tmp)
				comment += " " + tmp;
			comment.erase(0, 1);
		}
	}
	// :user2!root@127.0.0.1 PRIVMSG #chan :hi
	std::string	msg;
	msg = ":" + currClient.getNick() + ADR + " PRIVMSG " + channel + " :" + comment;
	sendMsgToChannelExceptMe(channel, msg, currClient);

	if (comment == ":letsGoClimbing();")
	{
		std::cout << "소환!\n";
		joinChannel(clients[getClientFdByNick("bot")], channel);
	}
}
