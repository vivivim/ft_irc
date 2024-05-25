#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
#include <vector>

class Client;

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
	std::vector<Client>		invitedPeople;

public:
	Channel();
	~Channel();
	
	bool	getIsLock();

};

#endif
