#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

#include <iostream>

void	Server::privmsg(std::stringstream& ss, Client currClient)
{
	std::string	dest;
	if (!(ss >> dest))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " PRIVMSG " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
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
	msg = ":" + currClient.getNick() + ADR + " PRIVMSG " + dest + " :" + comment;

	//존재하지않는채널&유저 예외처리
	if (dest[0] == '#')
	{
		sendMsgToChannelExceptMe(dest, msg, currClient);
		if (comment == ":letsGoClimbing();")
		{
			std::cout << "소환!\n";
			joinChannel(clients[getClientFdByNick("bot")], dest);
		}
	}
	else
		sendMsgToUser(dest, msg);
}
