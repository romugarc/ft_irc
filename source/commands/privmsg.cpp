#include "ft_irc.hpp"

void	RPRIVMSG(User *u1, User *u2, const std::string &target, const std::string &content);
void	E401(const std::string &host, User *u, const std::string &target_nick);
void 	E404(const std::string &host, User *u, Channel *c);
void	E411(const std::string &host, User *u, const std::string &cmd);
void	E412(const std::string &host, User *u);


static bool isInUserlist(std::deque<User*> users, std::string username)
{
	for (size_t i = 0; i < users.size(); i++)
	{
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
	std::deque<User*>		chanusers;
	User					*u_target = NULL;
	Channel					*c_target = NULL;
	bool					isuser = false;
	bool					ischan = false;

    if (user->getLoggedIn() == false)
	{
        return;
	}
	if (tokens.size() < 2)
	{
		E411(server->getHost(), user, "PRIVMSG");
		return;
	}
	if (tokens.size() < 3)
	{
		E412(server->getHost(), user);
		return;
	}

	userlist = server->getUsers();
	chanlist = server->getChannels();
	isuser = isInUserlist(userlist, tokens[1]);
	ischan = isInChanlist(chanlist, tokens[1]);
	if (!isuser && !ischan)
	{
		E401(server->getHost(), user, tokens[1]);
		return;
	}
	else if (ischan)
	{
		c_target = server->findChannel(tokens[1]);
		if (c_target->getModes().find("n") != std::string::npos && c_target->findUser(user->getFd()) == NULL)
		{
			E404(server->getHost(), user, c_target);
			return;
		}

		chanusers = c_target->getUserList();
		for (std::deque<User*>::iterator it = chanusers.begin(); it < chanusers.end(); it++)
		{
			if ((*it)->getFd() != user->getFd())
				RPRIVMSG((*it), user, tokens[1], tokens[2]);
		}
	}
	else
	{
		u_target = server->findUser(tokens[1]);
		RPRIVMSG(u_target, user, tokens[1], tokens[2]);
	}
}