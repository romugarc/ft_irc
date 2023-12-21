#include "ft_irc.hpp"

static bool isInUserlist(std::deque<User*> users, std::string username)
{
	std::cout << users.size() << std::endl;
	for (size_t i = 0; i < users.size(); i++)
	{
		std::cout << users[i]->getNick() << std::endl;
		if (username == users[i]->getNick())
			return true;
	}
	return false;
}

static bool isInChanlist(std::deque<Channel*> channels, std::string chan)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (chan == channels[i]->getName())
			return true;
	}
	return false;
}

void	privmsg(Server *server, User *user, std::deque<std::string> tokens)
{
	std::deque<User*>		userlist;
	std::deque<Channel*>	chanlist;
	User					*u_target = NULL;
	Channel					*c_target = NULL;
	bool					isuser = false;
	bool					ischan = false;

	if (tokens.size() < 2)
	{
		E411(user->getFd(), server->getHost(), user->getNick(), "PRIVMSG");
		return;
	}
	if (tokens.size() < 3)
	{
		E412(user->getFd(), server->getHost(), user->getNick());
		return;
	}

	userlist = server->getUsers();
	chanlist = server->getChannels();
	isuser = isInUserlist(userlist, tokens[1]);
	ischan = isInChanlist(chanlist, tokens[1]);
	std::cout << tokens[1] << ">> " << isuser << std::endl;
	if (!isuser && !ischan)
	{
		E401(user->getFd(), server->getHost(), user->getNick(), tokens[1]);
		return;
	}
	else if (ischan)
	{
		c_target = server->findChannel(tokens[1]);
		if (c_target->getModes().find("n") == std::string::npos && !c_target->findUser(user->getFd()))
		{
			E404(user->getFd(), server->getHost(), user->getNick(), c_target->getName());
			return;
		}

		for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end(); it++)
		{
			if ((*it)->getFd() != user->getFd())
				RPRIVMSG((*it)->getFd(), server->getHost(), user->getNick(), tokens[1], tokens[2]);
		}
	}
	else
	{
		u_target = server->findUser(tokens[1]);
		RPRIVMSG(u_target->getFd(), server->getHost(), user->getNick(), tokens[1], tokens[2]);
	}
}