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
		static Server*	instance;

		int			port;
		std::string	pwd;
		bool		isRunning;

		int								kq;
		std::vector<struct kevent>		changeList;
		struct kevent					eventList[8];
		Socket							socket;

		std::map<int, Client>			clients;
		std::map<std::string, Channel>	channels;
		std::queue<Response>			responses;
		
		void	createNewChannel(Client& newbie, std::string channelName);
		void	joinChannel(Client& newbie, std::string channelName);

	public:
		Server();
		Server(char *port, char *pwd);
		~Server();

		static void	signalHandler(int sigNum);
		void		closeTheDoor();

		void	create();
		void	run();
		void	changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
		void	disconnectClient(Client currClient);
		void	cleanChannel(std::string input);

		void	welcomeNewClient();
		void	getClientMsg(int currFd);
		void	sendResponseMsg();
		void	letsGoParsing(Client& currClient);
		void	sendWelcomeMsgToClient(Client& currClient);
		void	sendMsgToChannel(std::string channelName, std::string msg);
		void	sendMsgToUser(std::string dest, std::string msg);
		void	sendMsgToChannelExceptMe(std::string channelName, std::string msg, Client except);
		void	pushResponse(int fd, std::string msg);

		int		getPort();
		int		getClientFdByNick(std::string nick);

		void	pass(std::stringstream& ss, Client& currClient);
		void	user(std::stringstream& ss, Client& currClient);
		void	nick(std::stringstream& ss, Client& currClient);
		void	join(std::stringstream& ss, Client& currClient);
		void	privmsg(std::stringstream& ss, Client currClient);
		void	topic(std::stringstream& ss, Client& currClient);
		void	invite(std::stringstream& ss, Client& currClient);
		void	part(std::stringstream& ss, Client& currClient);
		void	quit(std::stringstream& ss, Client currClient);
		void	ping(std::stringstream& ss, Client currClient);
		void	kick(std::stringstream& ss, Client &currClient);
		void	mode(std::stringstream& ss, Client &currClient);
		bool	useNoOpAndSendMsgInMode(Client currClient, Channel currChannel, std::string channelName);
};
//util
std::vector<std::string> split(const std::string& str, const std::string& delimiter);

#endif
