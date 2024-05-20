#include "Client.hpp"

void	user(std::stringstream ss, Client currClient)
{
	if (currClient.getIsUsername())
		ERR_ALREADYREGISTERED(462);
}
