#include "ft_irc.hpp"

void	RQUIT(User *u1, User *u2, const std::string &comment);

void	quit(Server *server, User *user, std::deque<std::string> tokens)
{
	std::deque<User*> userlist = server->getUsers();
	std::string reason = "";

	if (tokens.size() > 1)
		reason = tokens[1];
	for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end(); it++)
		RQUIT((*it), user, reason);
	user->setQuit();
}

void	quit(Server *server, User *user)
{
	std::deque<User*> userlist = server->getUsers();
	
	for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end(); it++)
	{
		if ((*it)->getFd() != user->getFd())
			RQUIT((*it), user, "Connection lost");
	}
	user->setQuit();
}