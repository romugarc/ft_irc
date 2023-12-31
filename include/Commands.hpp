#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"

class   Server;
class	User;

void	pass(Server *server, User *user, std::deque<std::string> tokens);
void	nick(Server *server, User *user, std::deque<std::string> tokens);
void	user(Server *server, User *user, std::deque<std::string> tokens);
void    join(Server *server, User *user, std::deque<std::string> tokens);
void	mode(Server *server, User *user, std::deque<std::string> tokens);
void	kick(Server *server, User *user, std::deque<std::string> tokens);
void	invite(Server *server, User *user, std::deque<std::string> tokens);
void	topic(Server *server, User *user, std::deque<std::string> tokens);
void	privmsg(Server *server, User *user, std::deque<std::string> tokens);
void	quit(Server *server, User *user, std::deque<std::string> tokens);
void	quit(Server *server, User *user);
void	part(Server *server, User *user, std::deque<std::string> tokens);
void	whois(Server *server, User *user, std::deque<std::string> tokens);
void	notice(Server *server, User *user, std::deque<std::string> tokens);

#endif