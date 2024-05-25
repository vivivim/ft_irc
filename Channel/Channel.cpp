#include "Channel.hpp"

Channel::Channel() : memberCount(0), isLock(false), isInviteOnly(false), isLimit(false)
{
}

Channel::Channel(std::string name) : name(name), memberCount(0), isLock(false), inInviteOnly(false), isLimit(false)
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

void	plusMemberCount() { this->++memberCount; }
void	minusMemberCount() { this->--memberCount; }

int			Channel::getMemberCount() { return memberCount; }
int			Channel::getLimits() { return limits; }
std::string	Channel::getTopic() { return topic; }
std::string	Channel::getKey() { return key; }
bool		Channel::getIsLock() { return isLock; }
bool		Channel::getIsInviteOnly() { return isInviteOnly; }
bool		Channel::getIsLimit() { return isLimit; }
