#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <map>

class Channel;

class	Client {
	private:
		int								fd;
		std::string						msg;
		std::string						nick;
		std::string						username;
		std::map<std::string, Channel>	channels;

	public:
		Client();
		Client(int clientSocket, std::string msg);
		~Client();
		
		void		setMsg(std::string msg);
		void		attachMsg(std::string msg);
		std::string getMsg();
		int			getFd();
};

#endif
