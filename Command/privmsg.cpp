#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

#include <iostream>

void	Server::privmsg(std::stringstream& ss, Client currClient)
{
	std::string	input;
	if (!(ss >> input))
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

	std::vector<std::string> dests = split(input, ",");
	std::string	msg;
	for (size_t i = 0; i < dests.size(); ++i)
	{
		std::string dest = dests[i];
		msg = ":" + currClient.getNick() + ADR + " PRIVMSG " + dest + " :" + comment;

		if (dest[0] == '#')
		{
			if (channels.find(dest) == channels.end())
			{
				msg = IL + " " + ERR_NOSUCHCHANNEL + " "+ dest +  " " + ERR_NOSUCHCHANNEL_MSG;
				pushResponse(currClient.getFd(), msg);
				continue ;
			}
			sendMsgToChannelExceptMe(dest, msg, currClient);
			if (comment == ":letsGoClimbing();")
			{
				std::cout << "소환!\n";
				joinChannel(clients[getClientFdByNick("bot")], dest);
			}
		}
		else
		{
			if (getClientFdByNick(dest) == -1)
			{
				msg = IL + " " + ERR_NOSUCHNICK + " " + currClient.getNick() + " " + dest + " " + ERR_NOSUCHNICK_MSG;
				pushResponse(currClient.getFd(), msg);
				continue ;
			}
			sendMsgToUser(dest, msg);
		}
	}
}
