#include "Server.hpp"
#include "Command.hpp"
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
				sendResponseMsg(currEvent->ident);
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
		char	buf[40000];
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
		letsGoParsing(it->second);
	}
}

void	Server::letsGoParsing(Client& currClient)
{
	std::istringstream	iss(currClient.getMsg());
	std::string line;
	std::string	cmd;

	while (std::getline(iss, line))
	{
		std::stringstream	ss(line);
		ss >> cmd;
		if (cmd == "PASS")
			pass(ss, pwd, currClient);
		else if (cmd == "USER")
			user(ss, currClient);
		else if (cmd == "NICK")
			nick(ss, currClient, clients);
	}
}

void	Server::sendResponseMsg(int currFd)
{
	std::map<int, Client>::iterator it = clients.find(currFd);
	Client& currClient = it->second;
	if (it != clients.end())
	{
		if (!currClient.getMsg().empty())
		{
			int n = send(currFd, currClient.getMsg().c_str(), currClient.getMsg().size(), 0);
			if (n < 0)
			{
				std::cerr << "Error: Write failed\n";
				disconnectClient(currFd);
			}
			else
				currClient.setMsg("");
		}
	}
}

void	Server::disconnectClient(int key)
{
	close(key);
	clients.erase(key);
}

int		Server::getPort() { return port; }

