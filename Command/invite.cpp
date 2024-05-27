#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

#include <iostream>

void Server::invite(std::stringstream& ss, Client &currClient)
{
	std::string invitedUser;
	if (!(ss >> invitedUser)) // 무시
		return ;

	// 피초대자가 존재하지 않음 -> ERR_NOSUCHNICK (irssi 기준)
	if (clients.find(currClient.getFd()) == clients.end())
	{
		std::string msg = "401 " + currClient.getNick() + " " + invitedUser + " :No such nick/channel";
		pushResponse(currClient.getFd(), msg);
		return;
	}
	
	std::string channelName;
	if (!(ss >> channelName)) // 무시
		return;

	// 채널이 존재하지 않음 -> ERR_NOSUCHCHANNEL
	if (channels.find(channelName) == channels.end())
	{
		std::string msg = "403 " + currClient.getNick() + " " + invitedUser + " :No such channel";
		pushResponse(currClient.getFd(), msg);
		return;
	}

	// 초대자(currClient)가 채널의 구성원이 아님 -> ERR_NOTONCHANNEL
	Channel channel = channels.find(channelName)->second;
	if (!channel.IsUserInChannel(currClient.getNick()))
	{
		std::string msg = "442 " + currClient.getNick() + " " + channelName + " :You're not on that channel";
		pushResponse(currClient.getFd(), msg);
		return;
	}

	// inviteOnly인 경우, 초대자가 채널 운영자가 아님 -> ERR_CHANOPRIVSNEEDED
	if (channel.getIsInviteOnly() && channel.isChanOp(currClient.getNick()))
	{
		std::string msg = "482 " + currClient.getNick() + " " + channelName + " :You're not channel operator";
		pushResponse(currClient.getFd(), msg);
		return;
	}

	// 피초대자가 이미 채널에 존재 -> ERR_USERONCHANNEL
	if (channel.IsUserInChannel(invitedUser))
	{
		std::string msg = "443 " + currClient.getNick() + " " + invitedUser + " " + channelName + " :is already on channel";
		pushResponse(currClient.getFd(), msg);
		return;
	}
	
	// 초대 성공 -> RPL_INVITING : :dan-!d@localhost INVITE Wiz #test (dan-이 채널 test에 Wiz를 초대하는데에 성공함)
	channel.addInvited(invitedUser);
	Response response;
	std::string msg = "INVITE " + invitedUser + " " + channelName;
	response.setMsg(msg);
	responses.push(response);

	std::cout << "success invite\n";
}
