CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror
NAME = ircserv

CMDDIR = ./Command/

SRC = main.cpp \
	  ./Server/Server.cpp \
	  ./Socket/Socket.cpp \
	  ./Client/Client.cpp \
	  ./Channel/Channel.cpp \
	  ./Response/Response.cpp \
	  $(CMDDIR)nick.cpp $(CMDDIR)pass.cpp $(CMDDIR)user.cpp $(CMDDIR)join.cpp $(CMDDIR)topic.cpp $(CMDDIR)privmsg.cpp $(CMDDIR)invite.cpp $(CMDDIR)part.cpp $(CMDDIR)quit.cpp $(CMDDIR)ping.cpp $(CMDDIR)kick.cpp $(CMDDIR)mode.cpp

OBJS = $(SRC:.cpp=.o)
RM = rm -f

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:
	make fclean
	make all

.PHONY: make clean fclean all re
