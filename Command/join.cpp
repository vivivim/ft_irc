#include "../Command/Command.hpp"
#include <iostream>

void	Server::join(std::stringstream& ss, Client &currClient)
{
	std::string	inputChannels;

	if (!(ss >> inputChannels))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " JOIN " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::string inputKeys;
	ss >> inputKeys;
	std::vector<std::string> channelNames = split(inputChannels, ",");
	std::vector<std::string> keys = split(inputKeys, ",");
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::string chName = channelNames[i];
		if (channelNames[i][0] != '#')
			chName = "#" + channelNames[i];
		std::map<std::string, Channel>::iterator	it;
		it = channels.find(chName);
		if (it == channels.end())
			createNewChannel(currClient, chName);
		else
		{
			if (it->second.getIsLock())
			{
				if (i >= keys.size() || it->second.getKey() != keys[i])
				{
					std::string msg = IL + " " + ERR_BADCHANNELKEY + " " + currClient.getNick() + " " + chName + " " + ERR_BADCHANNELKEY_MSG;
					pushResponse(currClient.getFd(), msg);
					continue ;
				}
			}
			if (it->second.getIsInviteOnly())
			{
				if (!it->second.isSheInvited(currClient.getFd()))
				{
					std::string msg = IL + " " + ERR_INVITEONLYCHAN + " " + currClient.getNick() + " " + chName + " " + ERR_INVITEONLYCHAN_MSG;
					pushResponse(currClient.getFd(), msg);
					continue ;
				}
			}
			if (it->second.getIsLimit())
			{
				if (it->second.getLimits() <= it->second.getMemberCount())
				{
					std::string msg = IL + " " + ERR_CHANNELISFULL + " " + currClient.getNick() + " " + chName + " " + ERR_CHANNELISFULL_MSG;
					pushResponse(currClient.getFd(), msg);
					continue ;
				}
			}
			joinChannel(currClient, it->first);
		}
	}
}

void	Server::createNewChannel(Client& newbie, std::string channelName)
{
	Channel newChannel(channelName);
	newChannel.plusMemberCount();
	newChannel.addClient(newbie);
	newChannel.addOperator(newbie.getFd());
	newChannel.setCreatedTime();
	this->channels[channelName] = newChannel;
	
	std::string	msg = ":" + newbie.getNick() + ADR + newbie.getIPaddr() + " " + "JOIN " + channelName + "\r\n";
	msg += IL + " " + RPL_NAMREPLY + " " + newbie.getNick() + " = " + channelName + " :" + newChannel.getClientList() + "\r\n";
	msg += IL + " " + RPL_ENDOFNAMES + " " + newbie.getNick() + " " + channelName + " :End of /NAMES list.";
	pushResponse(newbie.getFd(), msg);
}

void	Server::joinChannel(Client& newbie, std::string channelName)
{
	Channel&	curr = channels[channelName];
	curr.addClient(newbie);
	curr.plusMemberCount();

	std::string	msg = ":" + newbie.getNick() + ADR + newbie.getIPaddr() + " " + "JOIN " + channelName + "\r\n";
	if (curr.getTopic() != "")
	{
		msg += IL + " " + RPL_TOPIC + " " + newbie.getNick() + " " + channelName + " :" + curr.getTopic() + "\r\n";
		msg += IL + " " + RPL_TOPICWHOTIME + " " + newbie.getNick() + " " + channelName + " " + curr.getTopicWho() + " :" + curr.getTopicTime() + "\r\n";
	}
	msg += IL + " " + RPL_NAMREPLY + " " + newbie.getNick() + " = " + channelName + " :" + curr.getClientList() + "\r\n";
	msg += IL + " " + RPL_ENDOFNAMES + " " + newbie.getNick() + " " + channelName + " :End of /NAMES list.";
	pushResponse(newbie.getFd(), msg);

	msg = ":" + newbie.getNick() + ADR + newbie.getIPaddr() + " JOIN :" + channelName;
	sendMsgToChannelExceptMe(channelName, msg, newbie);
}

