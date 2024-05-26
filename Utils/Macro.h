#ifndef MACRO_H
# define MACRO_H

# define SERVER_NAME "ircserv"

//WELCOME
# define RPL_WELCOME "001"
# define RPL_WELCOME_MSG "Welcome to the Internet Relay Network"
# define RPL_YOURHOST "002"
# define RPL_CREATED "003"
# define RPL_MYINFO "004"

//JOIN_REPLY
# define RPL_TOPIC "332"
# define RPL_TOPICWHOTIME "333"
# define RPL_NAMREPLY "353"
# define RPL_ENDOFNAMES "366"

//ERR
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_NONICKNAMEGIVEN_MSG ":No nickname given"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_ERRONEUSNICKNAME_MSG ":Erroneus nickname"
# define ERR_NICKNAMEINUSE "433"
# define ERR_NICKNAMEINUSE_MSG ":Nickname is already in use"

#endif
