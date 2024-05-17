#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class	Client {
	private:
		int			fd;
		std::string	msg;
		//channel info

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
