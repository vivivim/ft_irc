#include "../Server/Server.hpp"
#include "../Utils/Macro.h"
#include <sstream>

void	Server::part(std::stringstream& ss, Client& currClient)
{
	std::string	input;
	if (!(ss >> input))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	std::map<std::string, Channel>::iterator	it = channels.find(input);
	if (it == channels.end())
	{
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " " + currClient.getNick() + " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	if (!(it->second.IsUserInChannel(currClient.getNick())))
	{
		std::string msg = IL + " " + ERR_NOTONCHANNEL + " " + currClient.getNick() + " " + ERR_NOTONCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	std::string	msg;
	msg = ":" + currClient.getNick() + ADR + " PART :" + input + "\r\n\r\n";
	sendMsgToChannel(it->first, msg);

	Channel&	currChannel = it->second;
	currChannel.removeClient(currClient.getFd());
	if (!currChannel.getMemberCount())
		cleanChannel(input);
}
