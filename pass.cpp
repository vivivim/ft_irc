#include "Client.hpp"

void	pass(std::stringstream ss, std::string pwd, Client& currClient)
{
	std::string	input;
	if (currClient.getIsPass())
		ERR_ALREADYREGISTRED;
	if (!(ss >> input))
		ERR_NEEDMOREPARAMS;
	if (input == pwd)
		currClient.isPass = true;
}
