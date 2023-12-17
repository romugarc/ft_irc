
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

class Server;
class	User;

int join(Server *server, User *current_user, std::deque<std::string> tokens);

#endif