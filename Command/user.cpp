#include "../Command/Command.hpp"
#include <iostream>

void	Server::user(std::stringstream& ss, Client& currClient)
{
	std::cout << "in user\n";

	if (!currClient.getIsPass())
		return ;

	if (currClient.getIsUsername())
	{
		std::string msg = IL + " " + ERR_ALREADYREGISTRED + " " + currClient.getNick() + " " + ERR_ALREADYREGISTRED_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::string	input;
	if (!(ss >> input))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " " + input + " " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	if (input.size() > USERLEN)
		input = input.substr(0, 10);

	currClient.setUsername(input);
	currClient.setIsUsername(true);
	std::cout << "success user\n";
}
