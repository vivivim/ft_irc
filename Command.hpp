#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "macro.h"
# include "Client.hpp"
# include <sstream>
# include <string>
# include <map>

void	pass(std::stringstream& ss, std::string pwd, Client& currClient);
void	user(std::stringstream& ss, Client& currClient);
void	nick(std::stringstream& ss, Client& currClient, std::map<int, Client> clients);
void	join();
void	privmsg();
void	exit();

void	invite();
void	kick();
void	topic();
void	mode();

#endif
