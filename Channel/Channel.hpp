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
	std::vector<int>	invitedPeople;
	std::vector<int>	chanOpList; // 방장 닉네임 리스트
	std::string					createdTime;
	bool						isExistTopic;

public:
	Channel();
	Channel(std::string);
	~Channel();

	bool		isSheInvited(int fd);
	void		addClient(Client& newbie);
	void		removeClient(int fd);
	
	bool		IsUserInChannel(int fd);
	void		addInvited(int fd);
	bool		isChanOp(int fd);

	void		clean();

	void		plusMemberCount();
	void		minusMemberCount();

	void		setTopic(std::string input);
	void		setTopicWho(std::string input);
	void		setTopicTime();
	void		setCreatedTime();

	void		setIsInviteOnly(bool TF);
	void		setIsTopicOprOnly(bool TF);
	void		setIsLock(bool TF);
	void		setIsLimit(bool TF);
	void		setKey(std::string key);
	void		setLimits(int limits);
	void		setIsExistTopic(bool TF);

	void		addOperator(int fd);
	void		removeOperator(int fd);

	std::string	modeInfoToString();

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
	std::string	getCreatedTime();
	std::string	getlimitsToString(int limits);
	bool		getIsExistTopic();
};

#endif
