#include "ft_irc.hpp"

void	E461(const std::string &host, User *u, const std::string &cmd);
void	E462(const std::string &host, User *u);
void	E464(const std::string &host, User *u);

void	pass(Server *server, User *user, std::deque<std::string> tokens)
{
    if (tokens.size() <= 1 || tokens[1].empty()) //if not enough args
		E461(server->getHost(), user, "PASS");
	else if (user->getLoggedIn() == true) //if registration already done
		E462(server->getHost(), user);
	else if (tokens[1] != server->getPassword()) //if wrong password
    {
		E464(server->getHost(), user);
        user->setPass(false);
    }
    else
        user->setPass(true);
}