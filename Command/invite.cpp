#include "../Command/Command.hpp"

#include <iostream>

void invite(std::stringstream& ss, Client &currClient, std::map<int, Client> clients, std::map<std::string, Channel>& channels)
{
	std::string invitedUser;
	if (!(ss >> invitedUser)) // 무시
		return ;

	// 피초대자가 존재하지 않음 -> ERR_NOSUCHNICK (irssi 기준)
	if (clients.find(currClient.getFd()) == clients.end())
	{
		// ERR_NOSUCHNICK
		return;
	}
	
	std::string channelName;
	if (!(ss >> channelName)) // 무시
		return;

	// 채널이 존재하지 않음 -> ERR_NOSUCHCHANNEL
	if (channels.find(channelName) == channels.end())
	{
		// ERR_NOSUCHCHANNEL
		return;
	}

	// 초대자(currClient)가 채널의 구성원이 아님 -> ERR_NOTONCHANNEL
	Channel channel = channels.find(channelName)->second;
	if (!channel.IsUserInChannel(currClient.getNick()))
	{
		// ERR_NOTONCHANNEL
		return;
	}

	// inviteOnly인 경우, 초대자가 채널 운영자가 아님 -> ERR_CHANOPRIVSNEEDED
	if (channel.getIsInviteOnly() && channel.isChanOp(currClient.getNick()))
	{
		// ERR_CHANOPRIVSNEEDED
		return;
	}

	// 피초대자가 이미 채널에 존재 -> ERR_USERONCHANNEL
	if (channel.IsUserInChannel(invitedUser))
	{
		// ERR_USERONCHANNEL
		return;
	}
	
	// 초대 성공 -> RPL_INVITING : :dan-!d@localhost INVITE Wiz #test (dan-이 채널 test에 Wiz를 초대하는데에 성공함)
	channel.addInvited(invitedUser);
	std::string msg = "INVITE " + invitedUser + " " + channelName;
	// 응답메시지에 전송하기!

	std::cout << "success invite\n";
}
