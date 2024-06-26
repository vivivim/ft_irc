#include "../Command/Command.hpp"

void Server::kick(std::stringstream& ss, Client &currClient)
{
	std::string channelName;
	if (!(ss >> channelName))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " KICK " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::string userInput;
	if (!(ss >> userInput))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " KICK " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::string comment;
	if (ss >> comment)
	{
		if (comment[0] == ':')
		{
			std::string tmp;
			while (ss >> tmp)
				comment += " " + tmp;
			comment.erase(0, 1);
		}
	}

	// 채널이 존재하지 않음 403
	if (channels.find(channelName) == channels.end())
	{
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " "+ channelName +  " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::vector<std::string> users = split(userInput, ",");
	for (size_t i = 0; i < users.size(); ++i)
	{
		std::string user = users[i];
		// kick 당하는 닉네임의 클라이언트가 서버 자체에 존재하지 않음 401
		if (getClientFdByNick(user) == -1)
		{
			std::string msg = IL + " " + ERR_NOSUCHNICK + " " + currClient.getNick() + " " + user + " " + ERR_NOSUCHNICK_MSG;
			pushResponse(currClient.getFd(), msg);
			continue;
		}

		// currClient가 해당 채널에 존재하지 않음 442
		if (!channels[channelName].IsUserInChannel(currClient.getFd()))
		{
			std::string msg = IL + " " + ERR_NOTONCHANNEL + " " + currClient.getNick() + " " + channelName + " " + ERR_NOTONCHANNEL_MSG;
			pushResponse(currClient.getFd(), msg);
			continue;
		}

		// kick 당하는 user가 해당 존재하지 않음 441
		if (!channels[channelName].IsUserInChannel(currClient.getFd()))
		{
			std::string msg = IL + " " + ERR_USERNOTINCHANNEL + " " + currClient.getNick() + " " + user + " " + channelName + " " + ERR_USERNOTINCHANNEL_MSG;
			pushResponse(currClient.getFd(), msg);
			continue;
		}

		// currClient가 채널 운영자가 아님 482
		if (!channels[channelName].isChanOp(currClient.getFd()))
		{
			std::string msg = IL + " " + ERR_CHANOPRIVSNEEDED + " " + currClient.getNick() + " " + channelName + " " + ERR_CHANOPRIVSNEEDED_MSG;
			pushResponse(currClient.getFd(), msg);
			continue;
		}
		// kick 성공
		// 채널 사람들이 받는 응답 메시지 :user!root@127.0.0.1 KICK #a yj :
		std::string msg = ":" + currClient.getNick() + ADR + currClient.getIPaddr() + " KICK " + channelName + " " + user + " :" + comment;
		sendMsgToChannel(channelName, msg);
		channels[channelName].removeClient(getClientFdByNick(user));

		if (!(channels[channelName].getMemberCount()) || channels[channelName].isOnlyBot())
			cleanChannel(channelName);
	}
}
