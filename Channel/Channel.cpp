#include "Channel.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
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

void	Channel::removeClient(int fd)
{
	clients.erase(fd);
	minusMemberCount();
}

void	Channel::plusMemberCount() { ++memberCount; }
void	Channel::minusMemberCount() { --memberCount; }

void		Channel::setTopic(std::string input) { topic = input; }
void		Channel::setTopicWho(std::string input) { topicWho = input; }
void		Channel::setTopicTime()
{
	std::time_t	now = std::time(NULL);
	std::stringstream ss;
	ss << now;
	topicTime = ss.str();
}

void		Channel::setCreatedTime()
{
	std::time_t	now = std::time(NULL);
	std::stringstream ss;
	ss << now;
	createdTime = ss.str();
}

void		Channel::setIsInviteOnly(bool TF) { isInviteOnly = TF; }
void		Channel::setIsTopicOprOnly(bool TF) { isTopicOprOnly = TF; }
void		Channel::setIsLock(bool TF) { isLock = TF; }
void		Channel::setIsLimit(bool TF) { isLimit = TF; }
void		Channel::setKey(std::string key) { this->key = key; }
void		Channel::setLimits(std::string limitStr)
{
	int num = 0;
	for (int i = 0; i < limitStr.length(); ++i)
	{
		if ('0' <= limitStr[i] && limitStr[i] <= '9')
			num += limitStr[i] - '0';
	}
	this->limits = num;
}

void	Channel::addOperator(std::string nickName)
{
	chanOpList.push_back(nickName);
}

void	Channel::removeOperator(std::string nickName)
{
	std::vector<std::string>::iterator removeIter = std::remove(chanOpList.begin(), chanOpList.end(), nickName);
	chanOpList.erase(removeIter, chanOpList.end());
}

std::string	Channel::modeInfoToString()
{
	return " ";
}


std::string	Channel::getName() { return name; }
std::map<int, Client>&	Channel::getClients() { return clients; }
std::string	Channel::getClientList()
{
	std::string	list;
	std::map<int, Client>::iterator	it = clients.begin();
	for (; it != clients.end(); ++it)
	{
		if (isChanOp(it->second.getNick()))
			list += "@";
		list += it->second.getNick() + " ";
	}
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
std::string	Channel::getTopicWho() { return topicWho; }
std::string	Channel::getTopicTime() { return topicTime; }
std::string	Channel::getCreatedTime() { return createdTime; }