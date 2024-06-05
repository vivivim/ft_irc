#ifndef BOT_HPP
# define BOT_HPP

#include <string>
#include <vector>

class	Bot {
	private:
		int botSocket;
		static const int centerCnt = 5;
		std::vector<std::string> east;
		std::vector<std::string> west;
		std::vector<std::string> south;
		std::vector<std::string> north;

	public:
		Bot();
		Bot(char *port, char *pwd);
		~Bot();

		void	setClimbCenter();
		void	run();
		void	letsGoParsing(std::string buf);
		void	introduceBotself(std::stringstream& ss);
		void	parsingPrivmsg(std::stringstream& ss);
		void	sendMsgSet(std::string channelName, std::vector<std::string> msgSet);
};

#endif
