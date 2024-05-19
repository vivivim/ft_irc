#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include "Client.hpp"

class Channel
{
private:
	std::string				name;
	std::map<int, Client>	clients;
	int						memberCount;
	int						limits;
	std::string				topic;
	bool					inviteOnly;
	bool					isLock;
	std::string				key;

public:

};

#endif
