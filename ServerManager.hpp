#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <vector>

class	ServerManager {
	private:
		int							newEvents;
		std::vector<struct kevent>	changeList;
		struct kevent				eventList[8];
		Channel	channel;
		Command	cmd;
		Mode	mode;

	public:
		void	execute();
};
#endif
