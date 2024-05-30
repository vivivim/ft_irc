#include "../Server/Server.hpp"
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
	Channel&	currChannel = it->second;
	currChannel.removeClient(currClient.getFd());
	if (!currChannel.getMemberCount())
		cleanChannel(input);
}
