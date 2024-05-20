#include "Client.hpp"
#include "Config.h"

void	user(std::stringstream& ss, Client& currClient)
{
	if (currClient.getIsPass())
		return ;
	if (currClient.getIsUsername())
		ERR_ALREADYREGISTERED(462);

	std::string	input;
	if (!(ss >> input))
		ERR_NEEDMOREPARAMS(461);
	if (input.size() > USERLEN)
		input = input.substr(0, 10);
	CurrClient.setUsername(input);
	currClient.setIsUsername(true);
}
