#include "../Command/Command.hpp"
#include <iostream>

void	Server::pass(std::stringstream& ss, Client& currClient)
{
	std::cout << "in pass\n";
	std::string	input;

	if (currClient.getIsPass())
	{
		std::string msg = IL + " " + ERR_ALREADYREGISTRED + " " + currClient.getNick() + " " + ERR_ALREADYREGISTRED_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	if (!(ss >> input))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " " + input + " " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return;
	}
	if (input != pwd)
	{
		std::string msg = IL + " " + ERR_PASSWDMISMATCH + " " + currClient.getNick() + " " + ERR_PASSWDMISMATCH_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}
	currClient.setIsPass(true);
	std::cout << "success pass\n";
}
