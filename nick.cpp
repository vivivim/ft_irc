#include "Command.hpp"
#include "Client.hpp"
#include <string>
#include <sstream>
#include <set>

void nick(std::stringstream ss, Client &currClient, std::map<int, Client> clients)
{
	std::string cmd;
	std::string nick;
	ss >> cmd;
	if (ss.eof())
	{
		// ERR_NONICKNAMEGIVEN
		return;
	}
	ss >> nick;
	
	std::map<int, Client>::iterator it;
	std::string oldNick =  currClient.getNick();
	for(it = clients.begin(); it != clients.end(); ++it)
	{
		if (&it->second == &currClient)
			continue;
		if (it->second.getNick() == oldNick)
		{
			// ERR_NICKNAMEINUSE
			return;
		}

	}

	std::set<char> charSet = {':', '#'};
	for (size_t i = 0; i < nick.size(); ++i) {
		if (charSet.find(nick[i]) != charSet.end())
		{
			// ERR_ERRONEUSNICKNAME
			return;
		}
	}

	currClient.setNick(nick);
	// oldNick + "!" + root@127.0.0.1 + " NICK :" + nick
}