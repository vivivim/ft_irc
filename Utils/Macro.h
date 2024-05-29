#ifndef MACRO_H
#define MACRO_H

#include <string>

const std::string SERVER_NAME = "ircserv";

const std::string IL = ":irc.local";
const std::string ADR = "!root@127.0.0.1";

// WELCOME
const std::string RPL_WELCOME = "001";
const std::string RPL_WELCOME_MSG = "Welcome to the Internet Relay Network";
const std::string RPL_YOURHOST = "002";
const std::string RPL_CREATED = "003";
const std::string RPL_MYINFO = "004";

// JOIN_REPLY
const std::string RPL_TOPIC = "332";
const std::string RPL_TOPICWHOTIME = "333";
const std::string RPL_NAMREPLY = "353";
const std::string RPL_ENDOFNAMES = "366";

// INVITE_REPLY
const std::string RPL_INVITING = "341";

// ERR
const std::string ERR_NOSUCHNICK = "401";
const std::string ERR_NOSUCHNICK_MSG = ":No such nick/channel";
const std::string ERR_NOSUCHCHANNEL = "403";
const std::string ERR_NOSUCHCHANNEL_MSG = ":No such channel";
const std::string ERR_NONICKNAMEGIVEN = "431";
const std::string ERR_NONICKNAMEGIVEN_MSG = ":No nickname given";
const std::string ERR_ERRONEUSNICKNAME = "432";
const std::string ERR_ERRONEUSNICKNAME_MSG = ":Erroneus nickname";
const std::string ERR_NICKNAMEINUSE = "433";
const std::string ERR_NICKNAMEINUSE_MSG = ":Nickname is already in use";
const std::string ERR_NOTONCHANNEL = "442";
const std::string ERR_NOTONCHANNEL_MSG = ":You're not on that channel";
const std::string ERR_CHANOPRIVSNEEDED = "482";
const std::string ERR_CHANOPRIVSNEEDED_MSG = ":is already on channel";

#endif
