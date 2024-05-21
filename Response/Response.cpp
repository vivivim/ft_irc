#include "Response.hpp"

Response::Response()
{

}

Response::~Response()
{
	
}

void	Response::setFd(int fd)
{
	this->fd = fd;
}

void	Response::setMsg(std::string msg)
{
	this->msg = msg;
}


int	Response::getFd() { return fd; }

std::string	Response::getMsg() { return msg; };
