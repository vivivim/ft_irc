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
			//한번으로 끝인데 attach?
			it->second.attachMsg(buf);
		}
		std::cout << it->second.getMsg() << std::endl;
		letsGoParsing(it->second);
		sendWelcomeMsgToClient(it->second);
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
			pass(ss, currClient);
		else if (cmd == "USER")
			user(ss, currClient);
		else if (cmd == "NICK")
			nick(ss, currClient);
		else if (cmd == "JOIN")
			join(ss, currClient);
		else if (cmd == "PRIVMSG")
			privmsg(ss, currClient);
		else if (cmd == "TOPIC")
			topic(ss, currClient);
		else if (cmd == "INVITE")
			invite(ss, currClient);
		else if (cmd == "PART")
			part(ss, currClient);
		else if (cmd == "QUIT")
			quit(ss, currClient);
		else if (cmd == "PING")
			ping(ss, currClient);
		else if (cmd == "KICK")
			kick(ss, currClient);
		else if (cmd == "MODE")
			mode(ss, currClient);
	}
	currClient.setMsg("");
}

void	Server::sendMsgToChannel(std::string channelName, std::string msg)
{
	std::map<int, Client>			whoInChannel = channels[channelName].getClients();
	Channel channel = channels[channelName];
	std::map<int, Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (channel.IsUserInChannel(it->second.getNick()))
		{
			std::cout << "fd: " << it->first << "\n msg: " << msg << std::endl;
			pushResponse(it->first, msg);
		}
	}

}

void	Server::sendMsgToChannelExceptMe(std::string channelName, std::string msg, Client except)
{
	std::map<int, Client>			whoInChannel = channels[channelName].getClients();
	Channel channel = channels[channelName];
	std::map<int, Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (channel.IsUserInChannel(it->second.getNick()) && it->second.getNick() != except.getNick())
		{
			std::cout << "fd: " << it->first << "\n msg: " << msg << std::endl;
			pushResponse(it->first, msg);
		}
	}

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
		{
			std::cout << responses.front().getMsg() << std::endl;
			responses.pop();
		}
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
		std::string msg = IL + " " + RPL_WELCOME + " " + currClient.getNick() + " :Welcome to the Localnet IRC Network " + currClient.getNick() + ADR + "\r\n\r\n";
		// response.setMsg(":irc.local 002 yujin :Your host is irc.local, running version ircserv\r\n");
		// response.setMsg(":irc.local 003 yujin :This server was created 05:49:47 May 19 2024\r\n");

		currClient.setIsConnected(true);
		pushResponse(currClient.getFd(), msg);
		std::cout << "send welcome\n";
	}
}

void	Server::disconnectClient(int key)
{
	close(key);
	clients.erase(key);
}

void	Server::cleanChannel(std::string channelName)
{
	channels.erase(channelName);
}

int		Server::getPort() { return port; }


void	Server::pushResponse(int fd, std::string msg)
{
	Response response;
	response.setMsg(msg + "\r\n");
	response.setFd(fd);
	responses.push(response);
}

int	Server::getClientFdByNick(std::string nick)
{
	std::map<int, Client>::iterator	it;

	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNick() == nick)
			return it->second.getFd();
	}
	return -1;
}
