#include "../Command/Command.hpp"

void	Server::ping(std::stringstream& ss, Client currClient)
{
	std::string	input;
	if (!(ss >> input))
	{
		std::string msg = IL + " " + ERR_NEEDMOREPARAMS + " " + currClient.getNick() + " " + ERR_NEEDMOREPARAMS_MSG;
		pushResponse(currClient.getFd(), msg);
		return ;
	}

	std::string	msg = ":" + input + " PONG " + input + " :" + input;
	pushResponse(currClient.getFd(), msg);
}
