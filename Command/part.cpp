#include "../Command/Command.hpp"

void	Server::part(std::stringstream& ss, Client& currClient)
{
	std::string	channelName;
	if (!(ss >> channelName))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " " + ERR_NEEDMOREPARAMS_MSG;
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
	std::map<std::string, Channel>::iterator	it = channels.find(channelName);
	if (it == channels.end())
	{
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " " + currClient.getNick() + " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	if (!(it->second.IsUserInChannel(currClient.getFd())))
	{
		std::string msg = IL + " " + ERR_NOTONCHANNEL + " " + currClient.getNick() + " " + ERR_NOTONCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	std::string	msg;
	msg = ":" + currClient.getNick() + "!" + ADR + currClient.getIPaddr() + " PART " + channelName + " :" + comment;
	sendMsgToChannel(it->first, msg);

	Channel&	currChannel = it->second;
	currChannel.removeClient(currClient.getFd());
	if (!currChannel.getMemberCount() || currChannel.isOnlyBot())
		cleanChannel(channelName);
}
