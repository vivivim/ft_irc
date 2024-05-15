#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <>

class	Socket {
	private:
		int	socket;

	public:
		Socket();
		~Socket();

		void	bind();
		void	listen();
};

#endif
