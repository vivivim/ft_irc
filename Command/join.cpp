#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
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
		std::map<std::string, Channel>::iterator	it;
		it = channels.find(channelNames[i]);
		if (it == channels.end())
			createNewChannel(currClient, channelNames[i]);
		else
		{
			if (it->second.getIsLock())
			{
				std::cout << it->second.getKey() << " " << keys[i] << std::endl;
				if (i >= keys.size() || it->second.getKey() != keys[i])
				{
					std::string msg = IL + " " + ERR_BADCHANNELKEY + " " + currClient.getNick() + " " + channelNames[i] + " " + ERR_BADCHANNELKEY_MSG;
					pushResponse(currClient.getFd(), msg);
					continue ;
				}
			}
			if (it->second.getIsInviteOnly())
			{
				if (!it->second.isSheInvited(currClient.getNick()))
				{
					std::string msg = IL + " " + ERR_INVITEONLYCHAN + " " + currClient.getNick() + " " + channelNames[i] + " " + ERR_INVITEONLYCHAN_MSG;
					pushResponse(currClient.getFd(), msg);
					continue ;
				}
			}
			if (it->second.getIsLimit())
			{
				if (it->second.getLimits() <= it->second.getMemberCount())
				{
					std::string msg = IL + " " + ERR_CHANNELISFULL + " " + currClient.getNick() + " " + channelNames[i] + " " + ERR_CHANNELISFULL_MSG;
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
	newChannel.addOperator(newbie.getNick());
	newChannel.setCreatedTime();
	this->channels[channelName] = newChannel;
	
	std::string	msg = ":" + newbie.getNick() + ADR + " " + "JOIN " + channelName + "\r\n";
	msg += IL + " " + RPL_NAMREPLY + " " + newbie.getNick() + " = " + channelName + " :" + newChannel.getClientList() + "\r\n";
	msg += IL + " " + RPL_ENDOFNAMES + " " + newbie.getNick() + " " + channelName + " :End of /NAMES list.\r\n\r\n";
	pushResponse(newbie.getFd(), msg);
}

void	Server::joinChannel(Client& newbie, std::string channelName)
{
	Channel&	curr = channels[channelName];
	curr.addClient(newbie);
	curr.plusMemberCount();

	std::string	msg = ":" + newbie.getNick() + ADR + " " + "JOIN " + channelName + "\r\n";
	if (curr.getTopic() != "")
	{
		msg += IL + " " + RPL_TOPIC + " " + newbie.getNick() + " " + channelName + " :" + curr.getTopic() + "\r\n";
		msg += IL + " " + RPL_TOPICWHOTIME + " " + newbie.getNick() + " " + channelName + " " + curr.getTopicWho() + " :" + curr.getTopicTime() + "\r\n";
	}
	msg += IL + " " + RPL_NAMREPLY + " " + newbie.getNick() + " = " + channelName + " :" + curr.getClientList() + "\r\n";
	msg += IL + " " + RPL_ENDOFNAMES + " " + newbie.getNick() + " " + channelName + " :End of /NAMES list.\r\n\r\n";
	pushResponse(newbie.getFd(), msg);

	msg = ":" + newbie.getNick() + ADR + " JOIN :" + channelName + "\r\n\r\n";
	sendMsgToChannelExceptMe(channelName, msg, newbie);
}

