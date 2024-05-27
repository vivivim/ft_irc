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
		// ERR_NONICKNAMEGIVEN + oldNick + ERR_NONICKNAMEGIVEN_MSG (:irc.local 431 abc :No nickname given)
		return;
	}
	
	
	std::map<int, Client>::iterator it;
	for(it = clients.begin(); it != clients.end(); ++it)
	{
		if (&it->second == &currClient)
			continue;
		if (oldNick == nick)
		{
			// ERR_NICKNAMEINUSE + oldNcik + nick + ERR_NICKNAMEINUSE_MSG (:irc.local 433 origin nick :Nickname is already in use.)
			return;
		}
	}

	char elements[] = { ':', '#' };
	std::set<char> charSet(elements, elements + 2);
	for (size_t i = 0; i < nick.size(); ++i) {
		if (charSet.find(nick[i]) != charSet.end())
		{
			// ERR_ERRONEUSNICKNAME + oldNick + nick + ERR_ERRONEUSNICKNAME_MSG (:irc.local 432 w : :Erroneous Nickname)
			return ;
		}
	}

	currClient.setNick(nick);

	currClient.setIsNick(true);
	if (!currClient.getIsConnected())
		return ;

	std::string msg = "NICK :" + nick;
	pushResponse(currClient.getFd(), msg);

	// oldNick + "!" + root@127.0.0.1 + " NICK :" + nick
	std::cout << "success nick\n";
}
