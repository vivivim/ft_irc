#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

#include <iostream>

void Server::invite(std::stringstream& ss, Client &currClient)
{
	std::cout << "in invite\n";
	std::string invitedUser;
	if (!(ss >> invitedUser)) // 무시
		return ;

	// 피초대자가 존재하지 않음 (irssi 기준)
	if (getClientFdByNick(invitedUser) == -1)
	{
		std::string msg = IL + " " + ERR_NOSUCHNICK + " " + currClient.getNick() + " " + invitedUser + " " + ERR_NOSUCHNICK_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}
	
	std::string channelName;
	if (!(ss >> channelName)) // 무시
		return;

	// 채널이 존재하지 않음
	if (channels.find(channelName) == channels.end())
	{
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " "+ currClient.getNick() + " " + invitedUser + " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}

	// 초대자(currClient)가 채널의 구성원이 아님
	Channel& channel = channels.find(channelName)->second;
	if (!channel.IsUserInChannel(currClient.getFd()))
	{
		std::string msg = IL + " " + ERR_NOTONCHANNEL + " " + currClient.getNick() + " " + channelName + " " + ERR_NOTONCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}

	// inviteOnly인 경우, 초대자가 채널 운영자가 아님 -> ERR_CHANOPRIVSNEEDED
	if (channel.getIsInviteOnly() && !channel.isChanOp(currClient.getFd()))
	{
		std::string msg = IL + " " + ERR_CHANOPRIVSNEEDED + " " + currClient.getNick() + " " + channelName + " " + ERR_CHANOPRIVSNEEDED_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}

	// 피초대자가 이미 채널에 존재 -> ERR_USERONCHANNEL
	if (channel.IsUserInChannel(getClientFdByNick(invitedUser)))
	{
		std::string msg = IL + " " + ERR_USERONCHANNEL + " " + currClient.getNick() + " " + invitedUser + " " + channelName + " " + ERR_USERONCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}
	
	// 초대 성공
	channel.addInvited(getClientFdByNick(invitedUser));
	// 초대자가 받는 응답 메시지 -> :irc.local 341 user1 user3 :#b 
	std::string msg = IL + " " + RPL_INVITING + " " + currClient.getNick() + " " + invitedUser + " :" + channelName;
	pushResponse(currClient.getFd(), msg);
	// 피초대자가 받는 응답 메시지 -> user1!root@127.0.0.1 INVITE user3 :#b
	msg = ":" + currClient.getNick() + ADR + " INVITE " + invitedUser + " :" + channelName;
	pushResponse(getClientFdByNick(invitedUser), msg);
	
	std::cout << "success invite\n";
}
