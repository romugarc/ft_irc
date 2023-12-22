#include "ft_irc.hpp"

void	RKICK(User *u1, User *u2, Channel *c, const std::string &nick, const std::string &comment);
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void	E441(const std::string &host, User *u, Channel *c, const std::string &target_nick);
void	E442(const std::string &host, User *u, Channel *c);
void	E461(const std::string &host, User *u, const std::string &cmd);
void	E482(const std::string &host, User *u, Channel *c);

void	kick(Server *server, User *user, std::deque<std::string> tokens)
{
	Channel	*channel = NULL;
	User	*target = NULL;
	std::deque<User*>	userlist;
	std::string chan_name = "";
	std::string nick = "";
	std::string comment = "";

	if (tokens.size() > 1)
		chan_name = tokens[1];
	if (tokens.size() > 2)
		nick = tokens[2];
	if (tokens.size() > 3)
		comment = tokens[3];
	if (user->getLoggedIn() == false)
		return;
	if (tokens.size() < 3 || chan_name.empty())
	{
		E461(server->getHost(), user, "KICK");
		return;
	}

	channel = server->findChannel(chan_name);
	if (channel == NULL)
	{
		E403(server->getHost(), user, chan_name);
		return;
	}
	if (channel->findUser(user->getFd()) == NULL)
	{
		E442(server->getHost(), user, channel);
		return;
	}
	if (channel->findOperator(user->getFd()) == NULL)
	{
		E482(server->getHost(), user, channel);
		return;
	}

	target = channel->findUser(nick);
	if (target == NULL)
	{
		E441(server->getHost(), user, channel, nick);
		return;
	}
	
	if (channel->findUser(target->getFd()) != NULL)
	{
		userlist = channel->getUserList();
		for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end() ;it++)
			RKICK((*it), user, channel, nick, comment);
		channel->delOperator(target->getFd());
		channel->delUser(target->getFd());
	}
	
	if (channel->getNbUser() < 1)
		server->deleteChannel(channel->getName());
}