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
	std::string					topicTime;
	std::string					key;
	bool						isLock;
	bool						isInviteOnly;
	bool						isTopicOprOnly;
	bool						isLimit;
	std::vector<std::string>	invitedPeople;
	std::vector<std::string>	chanOpList; // 방장 닉네임 리스트

public:
	Channel();
	Channel(std::string);
	~Channel();

	bool		isSheInvited(std::string nick);
	void		addClient(Client& newbie);

	bool		IsUserInChannel(std::string nickName);
	void		addInvited(std::string nickName);
	bool		isChanOp(std::string nickName);

	void		removeClient(int fd);
	void		clean();

	void		plusMemberCount();
	void		minusMemberCount();

	void		setTopic(std::string input);
	void		setTopicWho(std::string input);
	void		setTopicTime();

	std::string	getName();
	std::map<int, Client>&	getClients();
	std::string	getClientList();
	int			getMemberCount();
	int			getLimits();
	std::string	getTopic();
	std::string	getTopicWho();
	std::string	getTopicTime();
	std::string	getKey();
	bool		getIsLock();
	bool		getIsInviteOnly();
	bool		getIsTopicOprOnly();
	bool		getIsLimit();
};

#endif
