#include "../Command/Command.hpp"
#include "../Client/Client.hpp"

#include <iostream>

void	pass(std::stringstream& ss, std::string pwd, Client& currClient)
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
