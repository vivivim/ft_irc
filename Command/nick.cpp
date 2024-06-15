#include "../Command/Command.hpp"
#include <set>
#include <iostream>

void Server::nick(std::stringstream& ss, Client &currClient)
{
	std::cout << "in nick\n";

	if (!currClient.getIsPass())
		return;

	std::string oldNick = currClient.getNick();
	std::string nick;
	if (!(ss >> nick))
	{
		std::string msg = IL + " " + ERR_NONICKNAMEGIVEN + " " + oldNick + " " + ERR_NONICKNAMEGIVEN_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}

	std::string botPwd;
	if (nick == "bot")
	{
		if (!(ss >> botPwd) || botPwd != "something")
		{
			std::cout << botPwd << "\n";
			std::string msg = IL + " " + ERR_ERRONEUSNICKNAME + " " + oldNick + " " + nick + " " + ERR_ERRONEUSNICKNAME_MSG;
			pushResponse(currClient.getFd(), msg);
			return;
		}
	}
	
	std::map<int, Client>::iterator it;
	for(it = clients.begin(); it != clients.end(); ++it)
	{
		if (&it->second == &currClient)
			continue;
		if (it->second.getNick() == nick)
		{
			std::string msg = IL + " " + ERR_NICKNAMEINUSE + " " + oldNick + " " + nick + " " + ERR_NICKNAMEINUSE_MSG;
			pushResponse(currClient.getFd(), msg);
			return;
		}
	}

	if (nick[0] == ':')
		nick.erase(0, 1);

	char elements[] = { '!', '@', '#', ':'};
	std::set<char> charSet(elements, elements + 4);
	for (size_t i = 0; i < nick.size(); ++i) {
		if (charSet.find(nick[i]) != charSet.end())
		{
			std::string msg = IL + " " + ERR_ERRONEUSNICKNAME + " " + oldNick + " " + nick + " " + ERR_ERRONEUSNICKNAME_MSG;
			pushResponse(currClient.getFd(), msg);
			return ;
		}
	}
	
	currClient.setNick(nick);
	currClient.setIsNick(true);
	if (!currClient.getIsConnected())
	{
		std::cout << "success nick in register\n";
		return ;
	}

	std::string msg = ":" + oldNick + ADR + currClient.getIPaddr() + " NICK :" + nick;

	std::set<int> connectedFd;
	std::map<std::string, Channel>::iterator	itChannel;
	connectedFd.insert(currClient.getFd());
	for (itChannel = channels.begin(); itChannel != channels.end(); ++itChannel)
	{
		if (itChannel->second.IsUserInChannel(currClient.getFd())) // 해당 사용자가 있는 채널
		{
			itChannel->second.addClient(currClient); //채널 클래스의 클라이언트 목록도 갱신
			std::map<int, Client> clientInChannel = itChannel->second.getClients();
			std::map<int, Client>::iterator itClient;
			for (itClient = clientInChannel.begin(); itClient != clientInChannel.end(); ++itClient) //해당 채널의 사용자 순회
				connectedFd.insert(itClient->first);
		}
	}
	
	std::set<int>::iterator itConnect;
	for(itConnect = connectedFd.begin(); itConnect != connectedFd.end(); ++itConnect)
		pushResponse(*itConnect, msg);

	std::cout << "success nick\n";
}
