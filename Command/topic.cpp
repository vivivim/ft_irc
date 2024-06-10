#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include <iostream>

void	Server::topic(std::stringstream& ss, Client& currClient)
{
	std::string	channelName;
	if (!(ss >> channelName))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " TOPIC " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	if (channels.find(channelName) == channels.end())
	{	
		std::string msg = IL + " " + ERR_NOSUCHCHANNEL + " "+ currClient.getNick() + " " + channelName + " " + ERR_NOSUCHCHANNEL_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}

	Channel&	currChannel = channels[channelName];
	std::string topic;
	if (!(ss >> topic)) // 이전에 설정된 토픽 확인
	{
		std::string msg;
		if (!currChannel.getIsExistTopic())
			msg = "No topic set for " + channelName;
		else
			msg = "Topic for " + channelName + " :" + currChannel.getTopic();
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	if (topic[0] == ':')
	{
		std::string tmp;
		while (ss >> tmp)
			topic += " " + tmp;
		topic.erase(0, 1);
	}
	if (currChannel.getIsTopicOprOnly() && currChannel.isChanOp(currClient.getNick()))
	{
		std::string msg = IL + " " + ERR_CHANOPRIVSNEEDED + " " + currClient.getNick() + " " + channelName + " " + ERR_CHANOPRIVSNEEDED_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}
	currChannel.setTopic(topic);
	currChannel.setIsExistTopic(true);
	currChannel.setTopicWho(currClient.getNick());
	currChannel.setTopicTime();
	//채널에 속한 사람 모두에게 응답 전송
	std::string	msg = ":" + currClient.getNick() + ADR + " TOPIC " + currChannel.getName() + " " + topic + "\r\n\r\n";
	sendMsgToChannel(currChannel.getName(), msg);
}
