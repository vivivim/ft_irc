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

		int								kq;
		std::vector<struct kevent>		changeList;
		struct kevent					eventList[8];
		Socket							socket;

		std::map<int, Client>			clients;
		std::map<std::string, Channel>	channels;
		std::queue<Response>			responses;
		// time_t					createdTime;
		
		void	createNewChannel(Client& newbie, std::string channelName);
		void	joinChannel(Client& newbie, std::string channelName);

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
		void	sendMsgToChannel(std::string channelName, std::string msg);
		void	sendMsgToChannelExceptMe(std::string channelName, std::string msg, Client except);

		void	topic(std::stringstream& ss, Client& currClient);
		void	join(std::stringstream& ss, Client &currClient);
		void	privmsg(std::stringstream& ss, Client currClient);
		
		int		getPort();
		int		getClientFdByNick(std::string nick);

		void	pushResponse(int fd, std::string msg);

		void	pass(std::stringstream& ss, Client& currClient);
		void	user(std::stringstream& ss, Client& currClient);
		void	nick(std::stringstream& ss, Client &currClient);
		void	invite(std::stringstream& ss, Client &currClient);
};

#endif
