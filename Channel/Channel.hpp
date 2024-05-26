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
	std::string					key;
	bool						isLock;
	bool						isInviteOnly;
	bool						isLimit;
	std::vector<std::string>	invitedPeople;
	bool						inviteOnly;
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
	
	void		plusMemberCount();
	void		minusMemberCount();

	int			getMemberCount();
	int			getLimits();
	std::string	getTopic();
	std::string	getKey();
	bool		getIsLock();
	bool		getIsInviteOnly();
	bool		getIsLimit();
};

#endif
