#include "Channel.hpp"
#include <algorithm>
#include "../Client/Client.hpp"

Channel::Channel() : memberCount(0), isLock(false), isInviteOnly(false), isLimit(false)
{
}

Channel::Channel(std::string name) : name(name), memberCount(0), isLock(false), isInviteOnly(false), isLimit(false)
{
}

Channel::~Channel()
{

}

bool		Channel::isSheInvited(std::string nick)
{
	std::vector<std::string>::iterator	it;
	it = find(invitedPeople.begin(), invitedPeople.end(), nick);
	if (it != invitedPeople.end())
		return true;
	return false;
}

void		Channel::addClient(Client& newbie)
{
	this->clients[newbie.getFd()] = newbie;
}

bool	Channel::IsUserInChannel(std::string nickName)
{
	std::map<int, Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNick() == nickName)
			return true;
	}
	return false;
}

void	Channel::addInvited(std::string nickName)
{
	// invite 메시지를 여러번 보낼 수 있어서 리스트 중복 체크
	if (std::find(invitedPeople.begin(), invitedPeople.end(), nickName) == invitedPeople.end())
	{
		invitedPeople.push_back(nickName);
	}
}

bool	Channel::isChanOp(std::string nickName)
{
	if (std::find(chanOpList.begin(), chanOpList.end(), nickName) == chanOpList.end())
		return false;
	return true;
}


void	Channel::plusMemberCount() { ++memberCount; }
void	Channel::minusMemberCount() { --memberCount; }

int			Channel::getMemberCount() { return memberCount; }
int			Channel::getLimits() { return limits; }
std::string	Channel::getTopic() { return topic; }
std::string	Channel::getKey() { return key; }
bool		Channel::getIsLock() { return isLock; }
bool		Channel::getIsInviteOnly() { return isInviteOnly; }
bool		Channel::getIsLimit() { return isLimit; }
