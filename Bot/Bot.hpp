#ifndef BOT_HPP
# define BOT_HPP

class	Bot {
	private:
		int botSocket;
	public:
		Bot();
		Bot(char *port, char *pwd);
		~Bot();

		void	run();
};

#endif
