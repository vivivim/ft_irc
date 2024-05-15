#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"

class	Server {
	private:
		//int? long?
		int			port;
		std::string	pwd;

		Socket		socket;
		//clients --> what name?
		int			kq;


	public:
		Server();
		Server(char *port, char *pwd);
		~Server();

		void	create();
		void	run();
};

#endif
