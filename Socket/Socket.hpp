#ifndef SOCKET_HPP
# define SOCKET_HPP

class	Socket {
	private:
		int	socket;

	public:
		Socket();
		~Socket();

		void	create();
		void	bind(int port);
		void	listen();

		int		getSocket();
};

#endif
