#include "Channel.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include "../Client/Client.hpp"

Channel::Channel() : memberCount(0), isLock(false), isInviteOnly(false), isTopicOprOnly(false), isLimit(false)
{
}

Channel::Channel(std::string name) : name(name), memberCount(0), isLock(false), isInviteOnly(false), isTopicOprOnly(false), isLimit(false), isExistTopic(false)
{
}

Channel::~Channel()
{

}

bool		Channel::isSheInvited(int fd)
{
	std::vector<int>::iterator	it;
	it = find(invitedPeople.begin(), invitedPeople.end(), fd);
	if (it != invitedPeople.end())
		return true;
	return false;
}

void	Channel::addClient(Client& newbie)
{
	clients[newbie.getFd()] = newbie;
}

bool	Channel::IsUserInChannel(int fd)
{
	std::map<int, Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getFd() == fd)
			return true;
	}
	return false;
}

void	Channel::addInvited(int fd)
{
	// invite 메시지를 여러번 보낼 수 있어서 리스트 중복 체크
	if (std::find(invitedPeople.begin(), invitedPeople.end(), fd) == invitedPeople.end())
	{
		invitedPeople.push_back(fd);
	}
}

bool	Channel::isChanOp(int fd)
{
	if (std::find(chanOpList.begin(), chanOpList.end(), fd) == chanOpList.end())
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
void		Channel::setLimits(int limits) { this->limits = limits; }
void		Channel::setIsExistTopic(bool TF) { this->isExistTopic = TF; }

void	Channel::addOperator(int fd)
{
	chanOpList.push_back(fd);
}

void	Channel::removeOperator(int fd)
{
	std::vector<int>::iterator removeIter = std::remove(chanOpList.begin(), chanOpList.end(), fd);
	chanOpList.erase(removeIter, chanOpList.end());
}

std::string	Channel::modeInfoToString()
{
	std::string modeInfo; // +iklt <key> <limits>
	std::string keyStr = "";
	std::string limitStr = "";
	modeInfo += "+";
	if (isInviteOnly)
		modeInfo += "i";
	if (isLock)
	{
		modeInfo += "k";
		keyStr = this->key;
	}
	if (isLimit)
	{
		modeInfo += "l";
		limitStr += getlimitsToString(this->limits);
	}
	if (isTopicOprOnly)
		modeInfo += "t";

	if (!keyStr.empty())
		modeInfo += " " + keyStr;
	if (!limitStr.empty())
		modeInfo += " " + limitStr;
	return modeInfo;
}


std::string	Channel::getName() { return name; }
std::map<int, Client>&	Channel::getClients() { return clients; }
std::string	Channel::getClientList()
{
	std::string	list;
	std::map<int, Client>::iterator	it = clients.begin();
	for (; it != clients.end(); ++it)
	{
		if (isChanOp(it->second.getFd()))
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
std::string	Channel::getlimitsToString(int limits)
{
	std::stringstream ss;
	ss << limits;
	return ss.str();
}

bool	Channel::getIsExistTopic() { return isExistTopic; }
