#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <map>
// #include "../Channel/Channel.hpp"

class	Client {
	private:
		int									fd;
		std::string							msg;
		std::string							prefix;
		std::string							nick;
		std::string							username;
		std::string							ip;

		bool								isPass;
		bool								isUsername;
		bool								isNick;
		bool								isConnected;

	public:
		Client();
		Client(int clientSocket, std::string msg, std::string ip);
		~Client();
		
		void		setMsg(std::string msg);
		void		attachMsg(std::string msg);
		void		setIsPass(bool TF);
		void		setIsUsername(bool TF);
		void		setIsNick(bool TF);
		void		setUsername(std::string input);
		void		setOperator();
		void		unsetOperator();
		void		setNick(std::string nick);
		void		setIsConnected(bool TF);

		int			getFd();
		std::string getMsg();
		std::string	getPrefix();
		bool		getIsPass();
		bool		getIsUsername();
		bool		getIsNick();
		bool		getIsConnected();
 		std::string	getNick();
		std::string	getIPaddr();
};

#endif
