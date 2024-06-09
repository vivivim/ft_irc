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
	east.push_back("클라이밍파크 성수점");
	east.push_back("알레클라이밍 강동점");
	east.push_back("온사이트클라이밍");
	east.push_back("더클라임 양재점");
	east.push_back("손상원클라이밍짐 강남점");

	// 서쪽 암장 추가
	west.push_back("더플라스틱 문래점");
	west.push_back("피커스 클라이밍 구로점");
	west.push_back("서울볼더스");
	west.push_back("볼더그라운드 클라이밍");
	west.push_back("더클라임 연남점");

	// 남쪽 암장 추가
	south.push_back("볼더생활");
	south.push_back("더클라임 신림점");
	south.push_back("정지현클라이밍짐");
	south.push_back("더클라임 사당점");
	south.push_back("더클라임 서울대점");

	// 북쪽 암장 추가
	north.push_back("서울숲클라이밍 종로점");
	north.push_back("을지로 담장");
	north.push_back("락랜드");
	north.push_back("손상원클라이밍짐 을지로점");
	north.push_back("피커스 클라이밍 종로점");
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

	msgSet.push_back(" : ___________________________________________________________________\r\n");
	msgSet.push_back(" : /                                                                   \\\r\n");
	msgSet.push_back(" : \\   안녕하세요? 저는 클라이밍 봇이에요.                                      /\r\n");
	msgSet.push_back(" : /   함께 클라이밍을 하러 떠나볼까요? 먼저 정보가 필요해요.                       \\\r\n");
	msgSet.push_back(" : \\   입력 메시지: #암장추천 <지역> <선호 무브> <실력>                          /\r\n");
	sendMsgSet(channelName, msgSet);
	msgSet.clear();

	msgSet.push_back(" : /   지역: 동서남북 / 선호 무브: 1. 2. 3. 4. 5. / 실력: 1(사람) ~ 5(원숭이)    \\\r\n");
	msgSet.push_back(" : \\   예시) #암장추천 서 3 5                                              /\r\n");
	msgSet.push_back(" : /  _________________________________________________________________\\\r\n");
	msgSet.push_back(" : | /\r\n");
	msgSet.push_back(" : |/\r\n");
	sendMsgSet(channelName, msgSet);
	msgSet.clear();

	// msgSet.push_back(" :안녕하세요! 저는 클라이밍 봇이랍니다~\r\n");
	// msgSet.push_back(" :함께 클라이밍을 하러 떠나볼까요? 먼저 정보가 필요해요.\r\n");
	// msgSet.push_back(" :입력 메시지: #암장추천 <지역> <선호 무브> <실력>\r\n");
	// msgSet.push_back(" :지역: 동서남북 / 선호 무브: 1.스태틱 2.코디 3.다이노 4.밸런스 5.오버행 / 실력: 1(사람) ~ 5(원숭이)\r\n");
	// msgSet.push_back(" :주의 사항: 항목 당 하나씩만 입력하세요.\r\n");
	// msgSet.push_back(" :예시) #암장추천 서 3 5\r\n");
	// sendMsgSet(channelName, msgSet);

	msgSet.push_back(" :|      __\r\n");
	msgSet.push_back(" :| w  c(..)o   (\r\n");
	msgSet.push_back(" :|  \\__(-)    __)\r\n");
	msgSet.push_back(" :|      /\\   (\r\n");
	msgSet.push_back(" :|     /(_)___)\r\n");
	msgSet.push_back(" :|       /|\r\n");
	msgSet.push_back(" :|      | \\\r\n");
	msgSet.push_back(" :|      m  m\r\n");
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
	if (id != "#암장추천")
		return ;
	
	srand(static_cast<unsigned int>(time(0)));
	int randNum = rand() % centerCnt;

	std::string result;
	std::string	loc;
	if (!(ss >> loc))
		return ;
	if (loc == "동")
		result = east[randNum];
	else if (loc == "서")
		result = west[randNum];
	else if (loc == "남")
		result = south[randNum];
	else if (loc == "북")
		result = north[randNum];
	else
		return;
	
	std::string	move;
	if (!(ss >> move))
		return ;
	if (!(move == "1" || move == "2" || move == "3" || move == "4" || move == "5"))
		return ;

	std::string	grade;
	if (!(ss >> grade))
		return ;
	if (!(grade == "1" || grade == "2" || grade == "3" || grade == "4" || grade == "5"))
		return ;

	std::vector<std::string> msgSet;
	msgSet.push_back(" :이 암장은 어떠세요?,,, -> " + result);
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