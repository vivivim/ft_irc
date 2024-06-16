#include "../Command/Command.hpp"

void	Server::pass(std::stringstream& ss, Client& currClient)
{
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
}
