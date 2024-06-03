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
	std::string opString;
	if (!(ss >> opString))
	{
		std::string msg = IL + " " + RPL_CHANNELMODEIS + " " + currClient.getNick() + " " + channelName + " " + currChannel.modeInfoToString();
		msg += IL + " " + RPL_CREATIONTIME + " " + currClient.getNick() + " " + channelName + " " + currChannel.getCreatedTime();
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	// 사용자에게 mode 변경 권한이 없음 -> ERR_CHANOPRIVSNEEDED(482) 
	if (!currChannel.isChanOp(currClient.getNick()))
	{
		std::string msg = IL + " " + ERR_CHANOPRIVSNEEDED + " " + currClient.getNick() + " " + channelName + " " + ERR_CHANOPRIVSNEEDED_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::vector<std::string> args;
	std::string arg;
	while (ss >> arg)
		args.push_back(arg);

	std::cout << "opString" << opString << std::endl;

	std::string modeResult;
	std::string modeResultArg;
	std::string msg;
	bool plus;
	size_t j = 0;
	for (size_t i = 0; opString.length(); i++)
	{
		if (opString[i] == '-' || opString[i] == '+')
		{
			modeResult += opString[i];
			plus = false;
			if (opString[i] == '+')
				plus = true;
			continue;
		}
		if (opString[i] == 'i' && currChannel.getIsInviteOnly() != plus) //초대받은 사람만 입장 가능
		{
			std::cout << "i\n";
			currChannel.setIsInviteOnly(plus);
			modeResult += "i";
		}
		else if (opString[i] == 't' && currChannel.getIsTopicOprOnly() != plus) //op만 TOPIC을 변경 가능
		{
			std::cout << "t\n";
			currChannel.setIsTopicOprOnly(plus);
			modeResult += "t";
		}
		else if (opString[i] == 'k' && currChannel.getIsLock() != plus) //채널 비밀번호 설정
		{
			std::cout << "k\n";
			if (plus) // 키를 새로 설정할 필요가 있으면 arg확인
			{
				if (j >= args.size())
				{
					// 알맞은 arg가 필요하다는 오류 메시지
					// msg += 
					continue;
				}
				std::string newKey = args[j++];
				currChannel.setKey(newKey);
				modeResultArg += " " + currChannel.getKey();
			}
			currChannel.setIsLock(plus);
			modeResult += "k";
		}
		else if (opString[i] == 'l') //채널 최대 인원 설정
		{
			std::cout << "l\n";
			if (plus) // 최대 인원을 새로 설정할 필요가 있으면 arg확인
			{
				if (j >= args.size())
				{
					// 알맞은 arg가 필요하다는 오류 메시지
					// msg += 
					continue;
				}
				std::string limitStr = args[j++];
				int newLimit = 0;
				for (size_t i = 0; i < limitStr.length(); ++i)
				{
					if ('0' <= limitStr[i] && limitStr[i] <= '9')
						newLimit += limitStr[i] - '0';
				}
				if (currChannel.getLimits() == newLimit) // limit가 그대로면, 응답메시지 작성 필요 없음
					continue;
				currChannel.setLimits(newLimit);
				modeResultArg += " " + currChannel.getlimitsToString(newLimit);
			}
			currChannel.setIsLimit(plus);
			modeResult += "l";
		}
		else if (opString[i] == 'o') //해당 닉네임 사용자에게 op권한 제공
		{
			std::cout << "o\n";
			if (j >= args.size())
			{
				// 알맞은 arg가 필요하다는 오류 메시지
				// msg += 
				continue;
			}
			std::string user = args[j++];
			if (getClientFdByNick(user) == -1) // 존재하지 않는 유저
			{
				msg += IL + " " + ERR_NOSUCHNICK + " " + currClient.getNick() + " " + user + " " + ERR_NOSUCHNICK_MSG;
				continue ;
			}
			if (currChannel.isChanOp(user) == plus) // 권한을 변경할 필요가 없는 유저
				continue ;
			if (plus)
				currChannel.addOperator(user);
			else
				currChannel.removeOperator(user);
			modeResult += "o";
			modeResultArg += " :" + user;
		}
		else // 알 수 없는 mode
			msg += IL + " " + ERR_UNKNOWNMODE + " " + currClient.getNick() + " " + opString[i] + " " + ERR_UNKNOWNMODE_MSG;
		std::cout << "modeResult : " << modeResult << std::endl;
	}
	if (!modeResult.empty())
		msg += ":" + currClient.getNick() + " " + ADR + " MODE " + channelName + " " + modeResult + modeResultArg;

	pushResponse(currClient.getFd(), msg);
	std::cout << "success mode\n";
}
