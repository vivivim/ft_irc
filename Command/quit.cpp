#include "../Command/Command.hpp"
#include "../Server/Server.hpp"

void	Server::quit(std::stringstream& ss, Client currClient)
{
	std::string	reason;
	std::string	tmp;
	std::string	msg;

	while (ss >> tmp)
		reason += tmp + " ";
	msg = " ERROR :Closing link: (root@127.0.0.1) [QUIT: " + reason + "]\r\n\r\n";
	pushResponse(currClient.getFd(), msg);
	msg = ":" + currClient.getNick() + ADR + " QUIT :Quit: " + reason + "\r\n\r\n";
	//channel이 없는데 우짜지
//	sendMsgToChannelExceptMe(channel, msg, currClient);
	disconnectClient(currClient.getFd());
}
