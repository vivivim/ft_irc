#include "Bot.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Follow this form --> ./bot <port> <password>" << std::endl;
		return 1;
	}
	try {
		Bot bot(argv[1], argv[2]);
		std::cout << "IRC bot is connected on port " << argv[1] << std::endl;
		bot.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
