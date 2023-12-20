#include "ft_irc.hpp"

void	E461(REP_ARG, const std::string &cmd);
void	E462(REP_ARG);
void	E464(REP_ARG);

void	pass(Server *server, User *user, std::deque<std::string> tokens)
{
    if (tokens.size() <= 1 || tokens[1].empty()) //if not enough args
		E461(user->getFd(), server->getHost(), user->getNick(), "PASS");
	else if (user->getLoggedIn() == true) //if registration already done
		E462(user->getFd(), server->getHost(), user->getNick());
	else if (tokens[1] != server->getPassword()) //if wrong password
    {
		E464(user->getFd(), server->getHost(), user->getNick());
        user->setPass(false);
    }
    else
    {
        //reussite
        user->setPass(true);
    }
}