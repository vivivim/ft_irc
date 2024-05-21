#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Socket/Socket.hpp"
# include "../Client/Client.hpp"
# include "../Channel/Channel.hpp"
# include "../Response/Response.hpp"
# include <string>
# include <map>
# include <vector>
# include <queue>
# include <sys/event.h>

class	Server {
	private:
		int			port;
		std::string	pwd;

		int						kq;
		std::vector<struct kevent>	changeList;
		struct kevent		eventList[8];
		Socket					socket;

		std::map<int, Client>	clients;
		std::map<int, Channel>	channels;
		std::queue<Response>	responses;
		// time_t					createdTime;

	public:
		Server();
		Server(char *port, char *pwd);
		~Server();

		void	create();
		void	run();
		void	changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
		void	disconnectClient(int key);
		void	welcomeNewClient();
		void	getClientMsg(int currFd);
		void	sendResponseMsg();
		void	letsGoParsing(Client& currClient);
		void	sendWelcomeMsgToClient(Client& currClient);
		
		int		getPort();
};

#endif
