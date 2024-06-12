#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include <iostream>

void	Server::quit(std::stringstream& ss, Client currClient)
{
	std::string	reason;
	std::string	tmp;
	std::string	msg;

	if (ss >> reason)
	{
		if (reason[0] == ':')
		{
			std::string tmp;
			while (ss >> tmp)
				reason += " " + tmp;
			reason.erase(0, 1);
		}
	}

	if (!reason.empty() && reason[0] == ':')
		reason = reason.substr(1);
	msg = ":" + currClient.getNick() + ADR + currClient.getIPaddr() + " QUIT :Quit: " + reason;
	std::map<std::string, Channel>::iterator	itChannel;
	std::vector<std::string>	removeCh;
	for (itChannel = channels.begin(); itChannel != channels.end(); ++itChannel)
	{
		if (itChannel->second.IsUserInChannel(currClient.getFd()))
		{
			sendMsgToChannelExceptMe(itChannel->second.getName(), msg, currClient);
			itChannel->second.removeClient(currClient.getFd());
			if (!(itChannel->second.getMemberCount()))
				removeCh.push_back(itChannel->second.getName());
		}
	}
	std::vector<std::string>::iterator it;
	for (it = removeCh.begin(); it != removeCh.end(); ++it)
		cleanChannel(*it);
	disconnectClient(currClient.getFd());
}
