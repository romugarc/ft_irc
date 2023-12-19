
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

class Server;
class	User;

void    join(Server *server, User *user, std::deque<std::string> tokens);
void	pass(Server *server, User *user, std::deque<std::string> tokens);
void	nick(Server *server, User *user, std::deque<std::string> tokens);
void	user(Server *server, User *user, std::deque<std::string> tokens);

#endif