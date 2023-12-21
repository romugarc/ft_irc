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
	std::cout << "1" << std::endl;
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
	std::cout << "2" << std::endl;
	user->setQuit();
}