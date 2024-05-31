#include "../Server/Server.hpp"
#include "../Utils/Macro.h"
#include <sstream>

void	Server::part(std::stringstream& ss, Client& currClient)
{
	std::string	input;
	if (!(ss >> input))
	{
		//ERR_NEEDMOREPARAMS(461);
		return ;
	}
	std::map<std::string, Channel>::iterator	it = channels.find(input);
	if (it == channels.end())
	{
		//ERR_NOSUCHCHANNEL(403);
		return ;
	}
	/*if (currClientNotInChannel())
	{
		//ERR_NOTONCHANNEL(442);
	}*/
	std::string	msg;
	msg = ":" + currClient.getNick() + ADR + " PART :" + input + "\r\n\r\n";
	sendMsgToChannel(it->first, msg);

	Channel&	currChannel = it->second;
	currChannel.removeClient(currClient.getFd());
	if (!currChannel.getMemberCount())
		cleanChannel(input);
}
