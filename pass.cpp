#include "Client.hpp"

void	pass(std::stringstream ss, std::string pwd, Client& currClient)
{
	std::string	input;
	if (currClient.getIsPass())
		ERR_ALREADYREGISTRED(462);
	if (!(ss >> input))
		ERR_NEEDMOREPARAMS(461);
	if (input == pwd)
		currClient.setIsPass(true);
}
