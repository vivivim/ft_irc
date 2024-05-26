#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
#include <vector>

class Client;

class Channel
{
private:
	std::string					name;
	std::map<int, Client>		clients;
	int							memberCount;
	int							limits;
	std::string					topic;
	std::string					topicWho;
	time_t						topicTime;
	std::string					key;
	bool						isLock;
	bool						isInviteOnly;
	bool						isTopicOprOnly;
	bool						isLimit;
	std::vector<std::string>	invitedPeople;

public:
	Channel();
	Channel(std::string);
	~Channel();
	
	bool		isSheInvited(std::string nick);
	void		addClient(Client& newbie);
	void		plusMemberCount();
	void		minusMemberCount();

	std::string	getClientList();
	int			getMemberCount();
	int			getLimits();
	std::string	getTopic();
	std::string	getTopicWho();
	std::string	getTopicTime();
	std::string	getKey();
	bool		getIsLock();
	bool		getIsInviteOnly();
	bool		getIsLimit();
};

#endif
