#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

#include <iostream>

bool	checkKickErr(std::string user)
{
	// 피초대자가 존재하지 않음 (irssi 기준)
	if (getClientFdByNick(kickedUser) == -1)
	{
		std::string msg = IL + " " + ERR_NOSUCHNICK + " " + currClient.getNick() + " " + kickedUser + " " + ERR_NOSUCHNICK_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}
	
	std::string channelName;
	if (!(ss >> channelName))
	{
		// ERR_NEEDMOREPARAMS
		return ;
	}

	// 채널이 존재하지 않음
	if (channels.find(channelName) == channels.end())
	{
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " "+ currClient.getNick() + " " + kickedUser + " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return true;
	}

	// currClient가 채널 운영자가 아님 -> ERR_CHANOPRIVSNEEDED
	if (channel.getIskickOnly() && channel.isChanOp(currClient.getNick()))
	{
		std::string msg = IL + " " + ERR_CHANOPRIVSNEEDED + " " + currClient.getNick() + " " + channelName + ERR_CHANOPRIVSNEEDED_MSG;
		pushResponse(currClient.getFd(), msg);
		return true;
	}

	// 쫓겨나는 사람이 채널에 존재하지 않음
	if (channel.IsUserInChannel(kickedUser))
	{
		std::string msg = IL + " " + ERR_NOTONCHANNEL + " " + kickedUser + " " + channelName + " " + ERR_NOTONCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return true;
	}
	return false;
}

void Server::kick(std::stringstream& ss, Client &currClient)
{
	std::cout << "in kick\n";
	std::string input;
	if (!(ss >> input))
	{
		// ERR_NEEDMOREPARAMS
		return ;
	}

	// ','로 user 파싱
	std::istringstream ssUser(kickedUsers);
	std::string user;

	// 입력받은 사용자별로 순회하면서 kick 진행
	while (std::getline(ssUser, user, ','))
	{
		if (checkErr(user))
			return;
		// kick 성공
		channel.addkickd(kickedUser);
		// 쫓아낸 사람이 받는 응답 메시지
		std::string msg = IL + " " + RPL_INVITING + " " + currClient.getNick() + " " + kickedUser + " :" + channelName;
		pushResponse(currClient.getFd(), msg);
		// 채널 사람들이 받는 응답 메시지   :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
		msg = ":" + currClient.getNick() + ADR + " kick " + kickedUser + " :" + channelName;
		pushResponse(getClientFdByNick(kickedUser), msg);
		std::cout << "success kick\n";
	}
}
