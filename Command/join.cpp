#include "Command.hpp"
#include "../Channel/Channel.hpp"

void	join(std::stringstream ss, std::map<int, Channel> channels)
{
	std::string	channelInput;

	if (!(ss >> channelInput))
	{
		// ERR_NEEDMOREPARAMS(461);
		return ;
	}
	if (channels.getIsLock())
	{
		std::string	keyInput;
		if (!(ss >> input))
		{
			//ERR_WHAT?
			return ;
		}
		if (keyInput == channels.getKey())
			입장;
		else
			//ERR_BADCHANNELKEY(475);
	}
	if (channels.getIsInviteOnly())
	{
		//invited 어떻게 확인? channel에 invited people 목록 만들기
		//혹은 사람에게 invited channel 목록 만들기
		if (IsInvited())
			입장;
		else
			//ERR_INVITEONLYCHAN(473);
	}
	if (channels.isLimit())
	{
		//isLimit을 만들거나 0과 양수로 구분
		//에러 확인하고 싶은데 지금은 귀찮아서 미루기
		if (channels.getLimits() > channels.getMemberCount())
			입장;
		else
			//ERR_CHANNELISFULL(471);
	}
	if (channels.find(channelInput) == channelInput.end())
		createNewChannel();
	else
		joinChannel();
}
