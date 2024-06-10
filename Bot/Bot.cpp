#include "Bot.hpp"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>

Bot::Bot()
{
}
Bot::Bot(char *portStr, char *pwdChar)
{
	int	port;
	std::istringstream	tmp(portStr);
	if (tmp >> port)
	{
		if (port < 0 || port > 65535)
			throw	std::runtime_error("Wrong port number");
	}
	else
		throw	std::runtime_error("Wrong port number");
	
	botSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (botSocket == -1)
		throw	std::runtime_error("Error: Could not create socket");
	
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

	if (connect(botSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
		throw	std::runtime_error("Error: Connection failed");

	std::string	msg;
	std::string pwd = pwdChar;
	msg = "PASS " + pwd + "\r\n";
	if (send(botSocket, msg.c_str(), msg.size(), 0) < 0)
	{
		//실패 시 종료? 아니면 계속 시도? 어떻게?
	}
	msg = "NICK bot\r\n";
	if (send(botSocket, msg.c_str(), msg.size(), 0) < 0)
		;
	msg = "USER bot\r\n";
	if (send(botSocket, msg.c_str(), msg.size(), 0) < 0)
		;
}

Bot::~Bot()
{
	close(botSocket);
}

void	Bot::setClimbCenter()
{
	// 동쪽 암장 추가
	east.push_back("Climbing Park Seongsu");
	east.push_back("Ale Climbing Gangdong");
	east.push_back("Onsite Climbing");
	east.push_back("The Climb Yangjae");
	east.push_back("SonSangwon Climbing Gym Gangnam");

	// 서쪽 암장 추가
	west.push_back("The Plastic Mullae");
	west.push_back("Peakers Climbing Guro");
	west.push_back("Seoul Boulders");
	west.push_back("Boulder Ground Climbing");
	west.push_back("The Climb Yeonnam");

	// 남쪽 암장 추가
	south.push_back("Boulder Life");
	south.push_back("The Climb Sillim");
	south.push_back("Jung Ji-hyun Climbing Gym");
	south.push_back("The Climb Sadang");
	south.push_back("The Climb Seoul National University");

	// 북쪽 암장 추가
	north.push_back("Seoul Forest Climbing Jongno");
	north.push_back("Euljiro Damjang");
	north.push_back("Rock Land");
	north.push_back("SonSangwon Climbing Gym Euljiro");
	north.push_back("Peakers Climbing Jongno");

}

void	Bot::run()
{
	while (true)
	{
		char buf[512];
		int n = read(botSocket, buf, sizeof(buf));
		std::string	readMsg;
		if (n < 0)
			throw	std::runtime_error("Error: Read failed");
		else
			buf[n] = '\0';
		std::cout << "readMsg: " << buf << std::endl;
		Bot::letsGoParsing(buf);
	}
}

void	Bot::letsGoParsing(std::string buf)
{
	std::string			cmd;
	std::stringstream	ss(buf);
	while (ss >> cmd)
	{
		if (cmd == "JOIN")
			introduceBotself(ss);
		else if (cmd == "PRIVMSG")
			parsingPrivmsg(ss);
	}
}

void	Bot::introduceBotself(std::stringstream& ss)
{
	std::string channelName;
	ss >> channelName;
	if (channelName[0] == ':')
		channelName.erase(0, 1);
	std::cout << "channelName : " << channelName << std::endl;
	std::vector<std::string> msgSet;

	msgSet.push_back(" :---------------------------------------------\r\n");
	msgSet.push_back(" : Hello! I'm the Climbing Bot.                \r\n");
	msgSet.push_back(" : If you're trying to decide                  \r\n");
	msgSet.push_back(" : which climbing gym to go to in Seoul,       \r\n");
	msgSet.push_back(" : Send me a message in the format:            \r\n");
	msgSet.push_back(" : #climb <Region: North, South, East, West>   \r\n");
	msgSet.push_back(" : ex) #climb West                             \r\n");
	msgSet.push_back(" :---------------------------------------------\r\n");
	sendMsgSet(channelName, msgSet);
	msgSet.clear();

	msgSet.push_back(" :      __\r\n");
	msgSet.push_back(" :  w  c(..)o   (\r\n");
	msgSet.push_back(" :   \\__(-)    __)\r\n");
	msgSet.push_back(" :       /\\   (\r\n");
	msgSet.push_back(" :      /(_)___)\r\n");
	msgSet.push_back(" :        /|\r\n");
	msgSet.push_back(" :       | \\\r\n");
	msgSet.push_back(" :       m  m\r\n");
	sendMsgSet(channelName, msgSet);
}

void	Bot::parsingPrivmsg(std::stringstream& ss)
{
	std::string	channelName;
	ss >> channelName;
	if (channelName[0] == ':')
		channelName.erase(0, 1);

	std::string	id;
	ss >> id;
	if (id[0] == ':')
		id.erase(0, 1);
	if (id != "#climb")
		return ;
	
	srand(static_cast<unsigned int>(time(0)));
	int randNum = rand() % centerCnt;

	std::string result;
	std::string	loc;
	if (!(ss >> loc))
		return ;
	if (loc == "East")
		result = east[randNum];
	else if (loc == "West")
		result = west[randNum];
	else if (loc == "South")
		result = south[randNum];
	else if (loc == "North")
		result = north[randNum];
	else
		return;

	std::vector<std::string> msgSet;
	msgSet.push_back(" :-----------------------------------\r\n");
	msgSet.push_back(" : How about this climbing gym? \r\n");
	msgSet.push_back(" :  -> " + result + "\r\n");
	msgSet.push_back(" :-----------------------------------\r\n");
	sendMsgSet(channelName, msgSet);
}

void	Bot::sendMsgSet(std::string channelName, std::vector<std::string> msgSet)
{
	for (size_t i = 0; i < msgSet.size(); ++i)
	{
		std::string msg = "PRIVMSG " + channelName + msgSet[i];
		if (send(botSocket, msg.c_str(), msg.size(), 0) < 0)
			throw	std::runtime_error("Error: Send failed");
	}
}