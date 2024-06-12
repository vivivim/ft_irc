#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
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

	// user1!root@127.0.0.1 NICK :u
	std::string msg = ":" + oldNick + ADR + currClient.getIPaddr() + " NICK :" + nick;
	pushResponse(currClient.getFd(), msg);

	std::cout << "success nick\n";
}
