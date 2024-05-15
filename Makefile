CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror
NAME = ircserv

SRC = main.cpp
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
