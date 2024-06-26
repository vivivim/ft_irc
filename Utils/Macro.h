#ifndef MACRO_H
#define MACRO_H

#include <string>

const std::string SERVER_NAME = "ircserv";

const std::string IL = ":irc.local";
const std::string ADR = "!root@";

// WELCOME
const std::string RPL_WELCOME = "001";
const std::string RPL_WELCOME_MSG = "Welcome to the Internet Relay Network";
const std::string RPL_YOURHOST = "002";
const std::string RPL_CREATED = "003";
const std::string RPL_MYINFO = "004";

// MODE_REPLY
const std::string RPL_CHANNELMODEIS = "324";
const std::string RPL_CREATIONTIME = "329";

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
const std::string ERR_USERNOTINCHANNEL = "441";
const std::string ERR_USERNOTINCHANNEL_MSG = ":They aren't on that channel";
const std::string ERR_NOTONCHANNEL = "442";
const std::string ERR_NOTONCHANNEL_MSG = ":You're not on that channel";
const std::string ERR_USERONCHANNEL = "443";
const std::string ERR_USERONCHANNEL_MSG = ":is already on channel";
const std::string ERR_NEEDMOREPARAMS = "461";
const std::string ERR_NEEDMOREPARAMS_MSG = ":Not enough parameters";
const std::string ERR_ALREADYREGISTRED = "462";
const std::string ERR_ALREADYREGISTRED_MSG = ":You may not reregister";
const std::string ERR_PASSWDMISMATCH = "464";
const std::string ERR_PASSWDMISMATCH_MSG = ":Password incorrect";
const std::string ERR_CHANNELISFULL = "471";
const std::string ERR_CHANNELISFULL_MSG = ":Cannot join channel (+l)";
const std::string ERR_UNKNOWNMODE = "472";
const std::string ERR_UNKNOWNMODE_MSG = ":is not a recognised channel mode.";
const std::string ERR_INVITEONLYCHAN = "473";
const std::string ERR_INVITEONLYCHAN_MSG = ":Cannot join channel (+i)";
const std::string ERR_BADCHANNELKEY = "475";
const std::string ERR_BADCHANNELKEY_MSG = ":Cannot join channel (+k)";
const std::string ERR_CHANOPRIVSNEEDED = "482";
const std::string ERR_CHANOPRIVSNEEDED_MSG = ":You're not channel operator";
const std::string ERR_INVALIDMODEPARAM = "696";
const std::string ERR_INVALIDMODEPARAM_MSG_LIMIT = " l * :You must specify a parameter for the limit mode. Syntax: <limit>.";
const std::string ERR_INVALIDMODEPARAM_MSG_KEY = " k * :You must specify a parameter for the key mode. Syntax: <key>.";
const std::string ERR_INVALIDMODEPARAM_MSG_NICK = " o * :You must specify a parameter for the op mode. Syntax: <nick>.";

#endif
