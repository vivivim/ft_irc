#include "./Server/Server.hpp"
#include <iostream>
#include <stdexcept>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Follow this form --> ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	try {
		Server	server(argv[1], argv[2]);
		server.create();
		std::cout << "IRC server is listening on port " << server.getPort() << std::endl;
		server.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
