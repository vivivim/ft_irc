#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>

#include <map>
#include <vector>


const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void	changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
	struct kevent	tempEvent;

	EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
	changeList.push_back(tempEvent);
}

void	disconnect_client(int client_fd, std::map<int, std::string> &clients)
{
	std::cout << "client disconnected: " << client_fd << std::endl;
	close(client_fd);
	clients.erase(client_fd);
}

int main() {
    // 소켓 생성
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error: Could not create socket\n";
        return EXIT_FAILURE;
    }

    // 소켓 주소 설정
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Bind failed\n";
        return EXIT_FAILURE;
    }

    // 클라이언트 연결 대기
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error: Listen failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Echo server is listening on port " << PORT << std::endl;

    int	kq = kqueue();
	if (kq == -1)
		kqueue() error;

	std::map<int, std::string>	clients;
	std::vector<struct kevent>	changeList;
	struct kevent				eventList[8];

	changeEvents(changeList, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	std::cout << "echo server with kqueue started '-'" << std::endl;
	
	int	newEvents;
	struct kevent	*currEvent;
	
	while (1)
	{
		newEvents = kevent(kq, &changeList[0], changeList.size(), eventList, 8, NULL);
		if (newEvents == -1)
			kevent error;
		changeList.clear();

		for (int i = 0; i < newEvents; ++i)
		{
			currEvent = &eventList[i];

			if (currEvent->flags & EV_ERROR)
			{
				error("currEvent->ident socket has problem o0o;;");
				if (currEvent->ident != server_socket)
					disconnect_client(currEvent->ident, clients);
			}
			else if (currEvent->filter == EVFILT_READ)
			{
				if (currEvent->ident == server_socket)
				{
					welcome new client();
				}
				else if (clients.find(currEvent->ident) != clients.end())
				{
					char	buf[1024];
					int		n = read(currEvent->ident, buf, sizeof(buf));

					if (n <= 0)
					{
						if (n < 0)
							error("currEvent->ident client socket has problem o0o;;");
						disconnect_client(currEvent->ident, clients);
					}
					else
					{
						buf[n] = '\0';
						client[currEvent->ident] += buf;
					}
				}
			}
			else if (currEvent->filter == EVFILT_WRITE)
			{
				std::map<int , std::string>::iterator it = clients.find(currEvent->ident);

				if (it != clients.end())
				{
					if (clients[currEvent->ident] != "")
					{
						int n = write(currEvent->ident, clients[currEvent->ident].c_str(), clients[currEvent->ident].size());
						if (n < 0)
						{
							error write
							disconnect_client(currEvent->ident, clients);
						}
						else
						{
							clients[currEvent->ident].clear();
						}
					}
				}
			}
		}

	}

    // 클라이언트 소켓 닫기
	std::map<int, std::string>::iterator	it = clients.begin();
	for (; it != clients.end(); ++it)
		close(it->first);
	clients.clear();

    // 서버 소켓 닫기
    close(server_socket);

    return EXIT_SUCCESS;
}

