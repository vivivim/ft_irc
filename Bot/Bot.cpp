#include "Bot.hpp"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>
#include <vector>

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

void	Bot::run()
{
	while (true)
	{
		// 응답 대기하다가 소환당하면 채널에 들어가기
		
		//안녕~ 자기소개
		//어느 지역? 동서남북 뭐를 선호? 1.스태틱 2.코디 3.다이노 4.밸런스 5.오버행 실력은? 1~10
		//ex) /암장추천 동 4 7
		//	  훅클라이밍 성수점은 어떠세요? ^_^
		//동 - 
		//서 - 
		//남 - 
		//북 -
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
	std::string			channel;
	std::string			privmsg;
	std::stringstream	ss(buf);
	while (ss >> cmd)
	{
		if (cmd == "JOIN")
		{
			std::cout << "join!\n";
			introduceBotself(ss);
		}
		else if (cmd == "PRIVMSG")
		{
			ss >> channel;
			ss >> privmsg;
			// parsingPrivmsg();
		}
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

	msgSet.push_back(" :안녕하세요! 저는 클라이밍 봇이랍니다~\r\n");
	msgSet.push_back(" :함께 클라이밍을 하러 떠나볼까요? 먼저 정보가 필요해요.\r\n");
	msgSet.push_back(" :입력 메시지: #암장추천 <지역> <선호 무브> <실력>\r\n");
	msgSet.push_back(" :지역: 동서남북 / 선호 무브: 1.스태틱 2.코디 3.다이노 4.밸런스 5.오버행 / 실력: 1(사람)~10(원숭이)\r\n");
	msgSet.push_back(" :주의 사항: 항목 당 하나씩만 입력하세요.\r\n");
	msgSet.push_back(" :예시) #암장추천 서 3 10\r\n");

	for (size_t i = 0; i < msgSet.size(); ++i)
	{
		std::string msg = "PRIVMSG " + channelName + msgSet[i];
		if (send(botSocket, msg.c_str(), msg.size(), 0) < 0)
			throw	std::runtime_error("Error: Send failed");
	}
}

void	Bot::parsingPrivmsg()
{
	
}