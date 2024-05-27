#include "Server.hpp"
#include "../Command/Command.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <cstdint>
#include <stdexcept>

Server::Server()
{
}

Server::Server(char *port, char *pwd)
{
	std::istringstream	tmp(port);
	if (tmp >> this->port)
	{
		if (this->port < 0 || this->port > 65535)
			throw	std::runtime_error("Wrong port number");
		this->pwd = pwd;
	}
	else
		throw	std::runtime_error("Wrong port number");	
}

Server::~Server()
{
	std::map<int, Client>::iterator it;

	for (it = clients.begin(); it != clients.end(); ++it)
		close(it->second.getFd());
	clients.clear();
}

void	Server::create()
{
	this->socket.create();
	socket.bind(this->port);
	socket.listen();
	this->kq = kqueue();
	if (this->kq == -1)
		throw	std::runtime_error("kqueue() failed");
	changeEvents(changeList, socket.getSocket(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

void Server::changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent tempEvent;
	EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
	changeList.push_back(tempEvent);
}

void	Server::run()
{
	int newEvents;
	struct kevent *currEvent;
	while (1)
	{
		newEvents = kevent(kq, &changeList[0], changeList.size(), eventList, 8, NULL);
		if (newEvents == -1)
			throw	std::runtime_error("kevent() failed");
		changeList.clear();

		for (int i = 0; i < newEvents; ++i)
		{
			currEvent = &eventList[i];
			if (currEvent->flags & EV_ERROR)
				std::cerr << "current event error" << std::endl;
			else if (currEvent->filter == EVFILT_READ)
			{
				if (currEvent->ident == static_cast<uintptr_t>(socket.getSocket()))
					welcomeNewClient();
				else
					getClientMsg(currEvent->ident);
			}
			else if (currEvent->filter == EVFILT_WRITE)
				sendResponseMsg();
		}
	}
}

void	Server::welcomeNewClient()
{
	int clientSocket = accept(socket.getSocket(), NULL, NULL);
	if (clientSocket < 0)
		std::cerr << "Client accept failed" << std::endl;
	else
		std::cout << "Accept client socket " << clientSocket << std::endl;
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	changeEvents(changeList, clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	changeEvents(changeList, clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	Client	freshClient(clientSocket, "");
	clients[clientSocket] = freshClient;
	//rpl_welcome
}

void	Server::getClientMsg(int currFd)
{
	std::map<int, Client>::iterator it = clients.find(currFd);
	if (it != clients.end())
	{
		char	buf[512];
		int n = recv(currFd, buf, sizeof(buf), 0);
		if (n <= 0)
		{
			if (n < 0)
				std::cerr << "Error: Client socket has problem\n";
			disconnectClient(currFd);
		}
		else
		{
			buf[n] = '\0';
			it->second.attachMsg(buf);
		}
		std::cout << it->second.getMsg() << std::endl;
		sendWelcomeMsgToClient(it->second);
		letsGoParsing(it->second);
	}
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != std::string::npos) {
		tokens.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}
	tokens.push_back(str.substr(start));

	return tokens;
}

void	Server::letsGoParsing(Client& currClient)
{
	std::string delimiter = "\r\n";
	std::string	cmd;
	std::vector<std::string> tokens = split(currClient.getMsg(), delimiter);

	for (size_t i = 0; i < tokens.size() && !tokens[i].empty(); ++i) {
		//std::cout << i << " " << tokens[i] << std::endl;
		std::stringstream	ss(tokens[i]);
		ss >> cmd;
		if (cmd == "PASS")
			pass(ss, pwd, currClient);
		else if (cmd == "USER")
			user(ss, currClient);
		else if (cmd == "NICK")
			nick(ss, currClient, clients);
		else if (cmd == "JOIN")
			join(ss, currClient);
		// else if (cmd == "CAP LS 302")
		// 	std::cout << "cap ls\n";
		// else if (cmd == "JOIN :")
		// 	std::cout << "join";
		else if (cmd == "PRIVMSG")
			privmsg(ss);
		else if (cmd == "topic")
			topic(ss, currClient);
	}
	currClient.setMsg("");
}

void	Server::topic(std::stringstream& ss, Client& currClient)
{
	std::string	input;
	if (!(ss >> input))
	{
		//ERR_NOTONCHANNEL(442);
		return ;
	}
	Channel	currChannel = channels[input];
	
	if (!(ss >> input))
	{
		//ERR_NEEDMOREPARAMS;
		return ;
	}
	if (currChannel.getIsTopicOprOnly() && currClient.getPrefix() != "@")
	{
		//ERR_CHANOPRIVSNEEDED(482);
		return ;
	}
	currChannel.setTopic(input);
	currChannel.setTopicWho(currClient.getNick());
	currChannel.setTopicTime();
	//채널에 속한 사람 모두에게 응답 전송
	std::string	msg = currClient.getNick() + " TOPIC " + currChannel.getName() + " :" + input + "\r\n\r\n";
	sendMsgToChannel(currChannel, msg);
}

void	Server::sendMsgToChannel(Channel channel, std::string msg)
{
	std::map<int, Client>	whoInChannel = channel.getClients();
	std::map<int, Client>::iterator	it = whoInChannel.begin();
	for (; it != whoInChannel.end(); ++it)
		pushResponse(it->first, msg);
}

void	Server::privmsg(std::stringstream& ss)
{
	std::string	channel;
	std::string	msg;
	if (!(ss >> channel))
	{
		//무슨 에러?
		return ;
	}
	if (!(ss >> msg))
	{
		//이런 일이 있을 수 있나?
		return ;
	}
	sendMsgToChannel(channel, msg);
}

void	Server::sendResponseMsg()
{
	while(!responses.empty())
	{
		int fd = responses.front().getFd();
		std::string msg = responses.front().getMsg();
		int n = send(fd, msg.c_str(), msg.size(), 0);
		if (n < 0)
		{
			std::cerr << "Error: Write failed\n";
			disconnectClient(fd);
		}
		else
			responses.pop();
	}
}

void	Server::sendWelcomeMsgToClient(Client& currClient)
{
	// if (!currClient.getIsPass() && !currClient.getIsUsername() && !currClient.getIsNick() && !currClient.getIsConnected())
	// {
	// 	Response response;
	// 	response.setMsg("NOTICE * :*** Looking up your hostname...\r\n451 * JOIN :You have not registered.\r\n\r\n");
	// 	response.setFd(currClient.getFd());
	// 	responses.push(response);
	// 	std::cout << "send welcome\n";
	// }

	if (currClient.getIsPass() && currClient.getIsUsername() && currClient.getIsNick() && !currClient.getIsConnected())
	{
		Response response;
		response.setMsg(":irc.local 001 yujin :Welcome to the Localnet IRC Network yujin!localhost@127.0.0.1\r\n");
		// response.setMsg(":irc.local 002 yujin :Your host is irc.local, running version ircserv\r\n");
		// response.setMsg(":irc.local 003 yujin :This server was created 05:49:47 May 19 2024\r\n");

		currClient.setIsConnected(true);
		response.setFd(currClient.getFd());
		responses.push(response);
		std::cout << "send welcome\n";
	}
}

void	Server::createNewChannel(Client& newbie, std::string channelName)
{
	Channel newChannel(channelName);
	newChannel.plusMemberCount();
	newChannel.addClient(newbie);
	newbie.setOperator();
	this->channels[channelName] = newChannel;
}

void	Server::joinChannel(Client& newbie, std::string channelName)
{
	Channel	curr = channels[channelName];
	curr.addClient(newbie);
	curr.plusMemberCount();
	std::string	msg = "JOIN " + channelName + "/r/n";
	if (curr.getTopic() != "")
	{
		msg += RPL_TOPIC + " " + newbie.getNick() + " " + channelName + " :" + curr.getTopic() + "/r/n";
		msg += RPL_TOPICWHOTIME + " " + newbie.getNick() + " " + channelName + " " + curr.getTopicWho() + " :" + curr.getTopicTime() + "\r\n";
	}
	msg += RPL_NAMREPLY + " " + newbie.getNick() + " = " + channelName + " :" + curr.getClientList() + "\r\n";
	msg += RPL_ENDOFNAMES + " " + newbie.getNick() + " " + channelName + " :End of /NAMES list.\r\n\r\n";
	pushResponse(newbie.getFd(), msg);
	msg = newbie.getNick() + " JOIN :" + channelName + "\r\n\r\n";
	sendMsgToChannel(channelName, msg);

}

void	Server::join(std::stringstream& ss, Client &currClient)
{
	std::string	channelInput;

	if (!(ss >> channelInput))
	{
		// ERR_NEEDMOREPARAMS(461);
		return ;
	}
	std::map<std::string, Channel>::iterator	it;
	it = channels.find(channelInput);
	if (it == channels.end())
		createNewChannel(currClient, channelInput);
	else
	{
		{
			std::string	keyInput;
			if (!(ss >> keyInput))
			{
				//ERR_NEEDMOREPARAMS (461);
				return ;
			}
			if (keyInput != it->second.getKey())
				return ;
				//ERR_BADCHANNELKEY(475);
		}
		if (it->second.getIsInviteOnly())
		{
			if (!it->second.isSheInvited(currClient.getNick()))
				return ;
				//ERR_INVITEONLYCHAN(473);
		}
		if (it->second.getIsLimit())
		{
			if (it->second.getLimits() <= it->second.getMemberCount())
				return ;
				//ERR_CHANNELISFULL(471);
		}
		joinChannel(currClient, channelInput);
	}
}

void	Server::disconnectClient(int key)
{
	close(key);
	clients.erase(key);
}

int		Server::getPort() { return port; }

