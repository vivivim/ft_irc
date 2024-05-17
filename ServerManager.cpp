#include "ServerManager.hpp"

void	ServerManager::init(int serverSocket)
{
	changeEvents(changeList, serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	newEvents = kevent(kq, &changeList[0], changeList.size(), eventList, 8, NULL);
	if (newEvents == -1)
	{
		std::cerr << "Error: kevent() failed\n";
		return EXIT_FAILURE;
	}
	changeList.clear();
}

void	ServerManager::execute()
{

	while (1)
	{
	}
}

