CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror
NAME = ircserv
BONUS_NAME = Bot/bot

CMDDIR = ./Command/

SRC = main.cpp \
	  ./Server/Server.cpp \
	  ./Socket/Socket.cpp \
	  ./Client/Client.cpp \
	  ./Channel/Channel.cpp \
	  ./Response/Response.cpp \
	  $(CMDDIR)nick.cpp $(CMDDIR)pass.cpp $(CMDDIR)user.cpp $(CMDDIR)join.cpp $(CMDDIR)topic.cpp $(CMDDIR)privmsg.cpp $(CMDDIR)invite.cpp $(CMDDIR)part.cpp $(CMDDIR)quit.cpp $(CMDDIR)ping.cpp $(CMDDIR)kick.cpp $(CMDDIR)mode.cpp \

BONUS_SRC = ./Bot/main.cpp \
			./Bot/Bot.cpp

OBJS = $(SRC:.cpp=.o)
BONUS_OBJS = $(BONUS_SRC:.cpp=.o)
RM = rm -f

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	$(CXX) $(CXXFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

bonus: $(BONUS_NAME)

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME)

re:
	make fclean
	make all

.PHONY: make clean fclean all re bonus
