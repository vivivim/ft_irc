#include "Bot.hpp"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>

Bot::Bot()
{
}
Bot::Bot(char *portStr, char *pwd)
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
}

Bot::~Bot()
{
	close(botSocket);
}

void	Bot::run()
{
	//채널에 들어가기
	
	//안녕~ 자기소개
	//어느 지역? 동서남북 뭐를 선호? 1.스태틱 2.코디 3.다이노 4.밸런스 5.오버행 실력은? 1~10
	//ex) /암장추천 동 4 7
	//	  훅클라이밍 성수점은 어떠세요? ^_^
	//동 - 
	//서 - 
	//남 - 
	//북 - 
}
