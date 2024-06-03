#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include <vector>
#include <utility>

#include <iostream>

void Server::mode(std::stringstream& ss, Client &currClient)
{
	std::cout << "in mode\n";
	
	std::string channelName;
	if (!(ss >> channelName))
		return ;

	// 채널이 존재하지 않음 -> ERR_NOSUCHCHANNEL
	if (channels.find(channelName) == channels.end())
	{
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " "+ channelName + " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}
	Channel& currChannel = channels.find(channelName)->second;

	// mode 옵션이 주어지지 않으면 해당 채널의 정보 반환 -> RPL_CHANNELMODEIS(324) + RPL_CREATIONTIME(329)
	std::string modeString;
	if (!(ss >> modeString))
	{
		std::string msg = IL + " " + RPL_CHANNELMODEIS + " " + currClient.getNick() + " " + channelName + " " + currChannel.modeInfoToString();
		msg += IL + " " + RPL_CREATIONTIME + " " + currClient.getNick() + " " + channelName + " " + currChannel.getCreatedTime();
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	// 사용자에게 mode 변경 권한이 없음 -> ERR_CHANOPRIVSNEEDED(482) 
	if (currChannel.isChanOp(currClient.getNick()))
	{
		std::string msg = IL + " " + ERR_CHANOPRIVSNEEDED + " " + currClient.getNick() + " " + channelName + " " + ERR_CHANOPRIVSNEEDED_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}

	std::vector<std::pair<char, char> > op;
	std::vector<std::string> arg;
	std::string tmp;
	while (!(ss >> tmp))
	{
		if (tmp[0] == '-' || tmp[0] == '+')
		{
			for (int i = 1; i < tmp.length(); ++i)
				op.push_back(std::make_pair(tmp[0], tmp[i]));
		}
		else
			arg.push_back(tmp);
	}

	std::string modeResult;
	std::string modeResultArg;
	std::string msg;
	int j = 0;
	for (int i = 0; op.size(); i++)
	{
		bool plus = true;
		if (op[i].first == '-')
			plus = false;

		if (op[i].second == 'i' && currChannel.getIsInviteOnly() != plus) //초대받은 사람만 입장 가능
		{
			// :user!root@127.0.0.1 MODE #nknk :-i
			currChannel.setIsInviteOnly(plus);
			modeResult += "i";
		}
		else if (op[i].second == 't' && currChannel.getIsTopicOprOnly() != plus) //op만 TOPIC을 변경 가능
			currChannel.setIsTopicOprOnly(plus);
		else if (op[i].second == 'k' && currChannel.getIsLock() != plus) //채널 비밀번호 설정
		{
			currChannel.setIsLock(plus);
			if (plus)
				currChannel.setKey(arg[j++]);
			// else
			// 	currChannel.setKey("");
		}
		else if (op[i].second == 'l' && currChannel.getIsLimit() != plus) //채널 최대 인원 설정
		{
			currChannel.setIsLimit(plus);
			if (plus)
				currChannel.setLimits(arg[j++]);
			// else
			// 	currChannel.setLimit(0);
		}
		else if (op[i].second == 'o') //해당 닉네임 사용자에게 op권한 제공
		{
			std::string user = arg[j++];
			if (getClientFdByNick(user) == -1) // 존재하지 않는 유저
			{
				msg += IL + " " + ERR_NOSUCHNICK + " " + currClient.getNick() + " " + user + " " + ERR_NOSUCHNICK_MSG;
				continue ;
			}
			if (currChannel.isChanOp(user) == plus) // 권한을 변경할 필요가 없는 유저
				continue ;
			if (op[i].first == '-')
				currChannel.removeOperator(user);
			else
				currChannel.addOperator(user);
		}
		else // 알 수 없는 mode
		{
			// :irc.local 472 user f :is not a recognised channel mode.
			msg += IL + " " + ERR_UNKNOWNMODE + " " + currClient.getNick() + " " + op[i].second + " " + ERR_UNKNOWNMODE_MSG;
		}
	}
	if (!modeResult.empty())
		msg += ":" + currClient.getNick() + " " + ADR + " MODE " + channelName + " " + modeResult + " :" + modeResultArg;

	pushResponse(currClient.getFd(), msg);
	std::cout << "success mode\n";
}
