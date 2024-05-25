#include "Channel.hpp"
#include <algorithm>

Channel::Channel()
{


}
Channel::~Channel()
{

}

bool	Channel::getIsLock() { return isLock; }

bool	Channel::getIsInviteOnly() { return inviteOnly; }

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