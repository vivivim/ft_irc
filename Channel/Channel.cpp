#include "Channel.hpp"
#include "../Client/Client.hpp"

Channel::Channel() : memberCount(0), isLock(false), isInviteOnly(false), isTopicOprOnly(false), isLimit(false)
{
}

Channel::Channel(std::string name) : name(name), memberCount(0), isLock(false), isInviteOnly(false), isTopicOprOnly(false), isLimit(false)
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

void	Channel::plusMemberCount() { ++memberCount; }
void	Channel::minusMemberCount() { --memberCount; }

void		Channel::setTopic(std::string input) { topic = input; }
void		Channel::setTopicWho(std::string input) { topicWho = input; }
void		Channel::setTopicTime()
{
	time_t	topicTime;
}

std::string	Channel::getName() { return name; }
std::map<int, Client>	Channel::getClients() { return clients; }
std::string	Channel::getclientList()
{
	std::string	list;
	std::map<int, Client>::iterator	it = clients.begin();
	for (; it != clients.end(); ++it)
		list += it->second.getPrefix + it->second.getNick() + " ";
	return list;
}

int			Channel::getMemberCount() { return memberCount; }
int			Channel::getLimits() { return limits; }
std::string	Channel::getTopic() { return topic; }
std::string	Channel::getKey() { return key; }
bool		Channel::getIsLock() { return isLock; }
bool		Channel::getIsInviteOnly() { return isInviteOnly; }
bool		Channel::getIsTopicOprOnly() { return isTopicOprOnly; }
bool		Channel::getIsLimit() { return isLimit; }
