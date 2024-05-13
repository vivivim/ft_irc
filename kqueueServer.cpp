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

void changeEvents(std::vector<struct kevent>& changeList, uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent tempEvent;
    EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
    changeList.push_back(tempEvent);
}

void disconnectClient(int clientFd, std::map<int, std::string> &clients)
{
    std::cout << "Client disconnected: " << clientFd << std::endl;
    close(clientFd);
    clients.erase(clientFd);
}

int main()
{
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Error: Could not create socket\n";
        return EXIT_FAILURE;
    }

    // Set socket address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error: Bind failed\n";
        return EXIT_FAILURE;
    }

    // Listen for client connections
    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Error: Listen failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Echo server is listening on port " << PORT << std::endl;

    int kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "Error: kqueue() failed\n";
        return EXIT_FAILURE;
    }

    std::map<int, std::string> clients;
    std::vector<struct kevent> changeList;
    struct kevent eventList[8];

    changeEvents(changeList, serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    std::cout << "Echo server with kqueue started '-'" << std::endl;

    int newEvents;
    struct kevent *currEvent;

    while (1)
    {

        newEvents = kevent(kq, &changeList[0], changeList.size(), eventList, 8, NULL);
        if (newEvents == -1)
        {
            std::cerr << "Error: kevent() failed\n";
            return EXIT_FAILURE;
        }
        changeList.clear();

        for (int i = 0; i < newEvents; ++i)
        {
            currEvent = &eventList[i];

            if (currEvent->flags & EV_ERROR)
            {
                std::cerr << "Error: Event error occurred\n";
                if (currEvent->ident != serverSocket)
                    disconnectClient(currEvent->ident, clients);
            }
            else if (currEvent->filter == EVFILT_READ)
            {
                if (currEvent->ident == serverSocket)
                {
                    std::cout << "Welcome new client\n";
                	int clientSocket = accept(serverSocket, NULL, NULL);
					if (clientSocket < 0)
						std::cerr << "Error: Accept failed\n";
					std::cout << "Accept client socket " << clientSocket << std::endl;
					fcntl(clientSocket, F_SETFL, O_NONBLOCK);
					changeEvents(changeList, clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					changeEvents(changeList, clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					clients[clientSocket] = "";
				}
                else if (clients.find(currEvent->ident) != clients.end())
                {
                    char buf[BUFFER_SIZE];
                    int n = recv(currEvent->ident, buf, sizeof(buf), 0);

                    if (n <= 0)
                    {
                        if (n < 0)
                            std::cerr << "Error: Client socket has problem\n";
                        disconnectClient(currEvent->ident, clients);
                    }
                    else
                    {
                        buf[n] = '\0';
                        clients[currEvent->ident] += buf;
                    }
                }
            }
            else if (currEvent->filter == EVFILT_WRITE)
            {
                std::map<int , std::string>::iterator it = clients.find(currEvent->ident);

                if (it != clients.end())
                {
                    if (!clients[currEvent->ident].empty())
                    {
                        int n = send(currEvent->ident, clients[currEvent->ident].c_str(), clients[currEvent->ident].size(), 0);
                        if (n < 0)
                        {
                            std::cerr << "Error: Write failed\n";
                            disconnectClient(currEvent->ident, clients);
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

    // Close client sockets
    std::map<int, std::string>::iterator it = clients.begin();
    for (; it != clients.end(); ++it)
        close(it->first);
    clients.clear();

    // Close server socket
    close(serverSocket);

    return EXIT_SUCCESS;
}

