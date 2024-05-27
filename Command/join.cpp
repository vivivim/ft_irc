#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

void	Server::join(std::stringstream& ss, Client &currClient)
{
	std::string	channelInput;

	if (!(ss >> channelInput))
	{
		// ERR_NEEDMOREPARAMS(461);
		return ;
	}
	std::map<std::string, Channel>::iterator	it;
	it = channels.find(channelInput);
	if (it == channels.end())
		createNewChannel(currClient, channelInput);
	else
	{
		{
			std::string	keyInput;
			if (!(ss >> keyInput))
			{
				//ERR_NEEDMOREPARAMS (461);
				return ;
			}
			if (keyInput != it->second.getKey())
				return ;
				//ERR_BADCHANNELKEY(475);
		}
		if (it->second.getIsInviteOnly())
		{
			if (!it->second.isSheInvited(currClient.getNick()))
				return ;
				//ERR_INVITEONLYCHAN(473);
		}
		if (it->second.getIsLimit())
		{
			if (it->second.getLimits() <= it->second.getMemberCount())
				return ;
				//ERR_CHANNELISFULL(471);
		}
		joinChannel(currClient, channelInput);
	}
}

void	Server::createNewChannel(Client& newbie, std::string channelName)
{
	Channel newChannel(channelName);
	newChannel.plusMemberCount();
	newChannel.addClient(newbie);
	newbie.setOperator();
	this->channels[channelName] = newChannel;
}

void	Server::joinChannel(Client& newbie, std::string channelName)
{
	Channel	curr = channels[channelName];
	curr.addClient(newbie);
	curr.plusMemberCount();
	std::string	msg = "JOIN " + channelName + "/r/n";
	if (curr.getTopic() != "")
	{
		msg += IL + " " + RPL_TOPIC + " " + newbie.getNick() + " " + channelName + " :" + curr.getTopic() + "/r/n";
		msg += IL + " " + RPL_TOPICWHOTIME + " " + newbie.getNick() + " " + channelName + " " + curr.getTopicWho() + " :" + curr.getTopicTime() + "\r\n";
	}
	msg += IL + " " + RPL_NAMREPLY + " " + newbie.getNick() + " = " + channelName + " :" + curr.getClientList() + "\r\n";
	msg += IL + " " RPL_ENDOFNAMES + " " + newbie.getNick() + " " + channelName + " :End of /NAMES list.\r\n\r\n";
	pushResponse(newbie.getFd(), msg);
	msg = newbie.getNick() + " JOIN :" + channelName + "\r\n\r\n";
	sendMsgToChannel(channelName, msg);

}
