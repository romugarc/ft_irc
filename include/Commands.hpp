
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

class Server;
class	User;

void	pass(Server *server, User *user, std::deque<std::string> tokens);
void	nick(Server *server, User *user, std::deque<std::string> tokens);
void	user(Server *server, User *user, std::deque<std::string> tokens);
void    join(Server *server, User *user, std::deque<std::string> tokens);
void	mode(Server *server, User *user, std::deque<std::string> tokens);
void	kick(Server *server, User *user, std::deque<std::string> tokens);

#endif