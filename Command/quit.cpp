#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

void	Server::quit(std::stringstream& ss, Client currClient)
{
	std::string	reason;
	std::string	tmp;
	std::string	msg;

	while (ss >> tmp)
		reason += tmp + " ";

	msg = ":" + currClient.getNick() + ADR + " QUIT :Quit: " + reason + "\r\n\r\n";
	std::map<std::string, Channel>::iterator	itChannel;
	for (itChannel = channels.begin(); itChannel != channels.end(); ++itChannel)
	{
		if (itChannel->second.IsUserInChannel(currClient.getNick()))
			sendMsgToChannelExceptMe(itChannel->second.getName(), msg, currClient);
	}
	disconnectClient(currClient.getFd());
}
