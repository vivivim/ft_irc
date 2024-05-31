#include "../Server/Server.hpp"
#include "../Command/Command.hpp"

void	Server::ping(std::stringstream& ss, Client currClient)
{
	std::string	input;
	if (!(ss >> input))
	{
		//ERR_NEEDMOREPARAMS(461);
		return ;
	}

	std::string	msg = ":" + input + " PONG " + input + " :" + input;
	pushResponse(currClient.getFd(), msg);
}
