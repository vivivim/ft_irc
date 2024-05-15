#include ""

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
		std::cout << "IRC server si listening on port " << server.getPort() << std::endl;
		server.run();
	}
	catch (const std::string *errMsg) {
		//Or std::exception ?
		std::cerr << errMsg << std::endl;
	}
	return 0;
}
