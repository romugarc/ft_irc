#include "ft_irc.hpp"

void	quit(Server *server, User *user, std::deque<std::string> tokens)
{
	std::deque<User*> userlist = server->getUsers();
	
	for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end(); it++)
	{
		if (tokens.size() > 1)
			RQUIT((*it)->getFd(), server->getHost(), user->getNick(), tokens[1]);
		else
			RQUIT((*it)->getFd(), server->getHost(), user->getNick(), "");
	}
	user->setQuit();
}

void	quit(Server *server, User *user)
{
	std::deque<User*> userlist = server->getUsers();
	
	for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end(); it++)
	{
		if ((*it)->getFd() != user->getFd())
			RQUIT((*it)->getFd(), server->getHost(), user->getNick(), "Connection lost");
	}
	user->setQuit();
}