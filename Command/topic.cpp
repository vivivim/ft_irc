#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include <iostream>

void	Server::topic(std::stringstream& ss, Client& currClient)
{
	std::string	input;
	if (!(ss >> input))
	{
		//ERR_NOTONCHANNEL(442);
		//NEEDMOREPARAMS?
		return ;
	}
	if (channels.find(input) == channels.end())
	{	
		//ERR_NOSUCHCHANNEL;
		return ;
	}

	Channel&	currChannel = channels[input];
	if (!(ss >> input))
	{
		//ERR_NEEDMOREPARAMS;
		return ;
	}
	if (currChannel.getIsTopicOprOnly() && currClient.getPrefix() != "@")
	{
		//ERR_CHANOPRIVSNEEDED(482);
		return ;
	}
	currChannel.setTopic(input);
	currChannel.setTopicWho(currClient.getNick());
	currChannel.setTopicTime();
	//채널에 속한 사람 모두에게 응답 전송
	std::string	msg = ":" + currClient.getNick() + ADR + " TOPIC " + currChannel.getName() + " " + input + "\r\n\r\n";
	sendMsgToChannel(currChannel.getName(), msg);
}
