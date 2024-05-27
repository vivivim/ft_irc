#include "../Command/Command.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"

#include <iostream>

void	Server::pass(std::stringstream& ss, Client& currClient)
{
	std::string	cmd;
	std::string	input;

	if (currClient.getIsPass())
		// ERR_ALREADYREGISTRED(462);
	ss >> cmd;
	if (!(ss >> input))
	{
		// ERR_NEEDMOREPARAMS(461);
		return;
	}
	if (input != pwd)
	{
		// ERR_PASSWDMISMATCH (464) ":Password incorrect
		return ;
	}
	currClient.setIsPass(true);
	std::cout << "success pass\n";
}
