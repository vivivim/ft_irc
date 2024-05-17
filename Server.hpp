#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
# include "Client.hpp"
# include <string>
# include <vector>
# include <sys/event.h>

class	Server {
	private:
		int			port;
		std::string	pwd;

		int					kq;
		Socket				socket;
		std::vector<Client>	clients;
//		ServerManager		serverManager;
		std::vector<struct kevent>	changeList;
		struct kevent		eventList[8];

	public:
		Server();
		Server(char *port, char *pwd);
		~Server();

		void	create();
		void	run();
		void	changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
		std::vector<Client>::iterator	find(int socketClient);
		void	disconnectClient(std::vector<Client>::iterator it);
		
		int		getPort();
};
		

#endif
