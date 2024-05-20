#include "Command.hpp"
#include "Client.hpp"
#include <string>
#include <sstream>
#include <set>

void nick(std::stringstream& ss, Client &currClient, std::map<int, Client> clients)
{
	if (currClient.getIsPass() == false)
		return;

	std::string oldNick = currClient.getNick();
	std::string cmd;
	std::string nick;
	ss >> cmd;
	if (ss.eof())
	{
		// ERR_NONICKNAMEGIVEN + oldNick + ERR_NONICKNAMEGIVEN_MSG (:irc.local 431 abc :No nickname given)
		return;
	}
	ss >> nick;
	
	std::map<int, Client>::iterator it;
	for(it = clients.begin(); it != clients.end(); ++it)
	{
		if (&it->second == &currClient)
			continue;
		if (it->second.getNick() == oldNick)
		{
			// ERR_NICKNAMEINUSE + oldNcik + nick + ERR_NICKNAMEINUSE_MSG (:irc.local 433 origin nick :Nickname is already in use.)
			return;
		}

	}

	std::set<char> charSet = {':', '#'};
	for (size_t i = 0; i < nick.size(); ++i) {
		if (charSet.find(nick[i]) != charSet.end())
		{
			// ERR_ERRONEUSNICKNAME + oldNick + nick + ERR_ERRONEUSNICKNAME_MSG (:irc.local 432 w : :Erroneous Nickname)
			return;
		}
	}

	currClient.setNick(nick);
	currClient.setIsNick(true);
	// oldNick + "!" + root@127.0.0.1 + " NICK :" + nick
}