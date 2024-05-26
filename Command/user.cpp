#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../Utils/Config.h"

#include <iostream>

void	Server::user(std::stringstream& ss, Client& currClient)
{
	std::cout << "in user\n";

	if (!currClient.getIsPass())
		return ;
	if (currClient.getIsUsername())
	{
		// ERR_ALREADYREGISTERED(462);
		return ;
	}

	std::string	input;
	if (!(ss >> input))
	{
		// ERR_NEEDMOREPARAMS(461);
		return ;
	}
	if (input.size() > USERLEN)
		input = input.substr(0, 10);
	currClient.setUsername(input);
	currClient.setIsUsername(true);
	std::cout << "success user\n";
}
