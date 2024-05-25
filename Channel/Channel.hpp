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
	bool						inviteOnly;
	bool						isLock;
	std::string					key;
	std::vector<std::string>	invitedPeople;
	std::vector<std::string>	chanOpList; // 방장 닉네임 리스트

public:
	Channel();
	~Channel();
	
	bool	getIsLock();
	bool	getIsInviteOnly();

	bool	IsUserInChannel(std::string nickName);
	bool	isChanOp(std::string nickName);
	void	addInvited(std::string nickName);
};

#endif
