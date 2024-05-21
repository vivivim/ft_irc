#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>

class Response
{
private:
	int			fd;
	std::string	msg;

public:
	Response();
	~Response();

	void	setFd(int fd);
	void	setMsg(std::string	msg);

	int			getFd();
	std::string	getMsg();
};

#endif
