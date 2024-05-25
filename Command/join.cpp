#include "Command.hpp"
#include "../Channel/Channel.hpp"

void	join(std::stringstream ss, Client &currClient, std::map<std::string, Channel> channels)
{
	std::string	channelInput;

	if (!(ss >> channelInput))
	{
		// ERR_NEEDMOREPARAMS(461);
		return ;
	}
	std::map<std::string, Channel>::iterator	it;
	it = channels.find(channelInput);
	if (it == channelInput.end())
		createNewChannel();
	else
	{
		if (it->second.getIsLock())
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
		if (it->second.isLimit())
		{
			if (it->second.getLimits() <= it->second.getMemberCount())
				return ;
				//ERR_CHANNELISFULL(471);
		}
		joinChannel(currClient, channelInput);
	}
}
