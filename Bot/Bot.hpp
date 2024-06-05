#ifndef BOT_HPP
# define BOT_HPP

#include <string>

class	Bot {
	private:
		int botSocket;
	public:
		Bot();
		Bot(char *port, char *pwd);
		~Bot();

		void	run();
		void	letsGoParsing(std::string buf);
		void	introduceBotself(std::stringstream& ss);
		void	parsingPrivmsg();
};

#endif
