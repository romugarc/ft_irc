#include "ft_irc.hpp"

void 	E403(REP_ARG, const std::string &channel);
void	E441(REP_ARG, const std::string &channel, const std::string &input_name);
void	E442(REP_ARG, const std::string &channel);
void	E461(REP_ARG, const std::string &cmd);
void	E482(REP_ARG, const std::string &channel);
void	RKICK(REP_ARG, const std::string &channel, const std::string &nick, const std::string &comment);

void	kick(Server *server, User *user, std::deque<std::string> tokens)
{
	Channel	*channel = NULL;
	User	*target = NULL;
	std::deque<User*>	userlist;

	if (user->getLoggedIn() == false)
		return;
	if (tokens.size() < 3 || tokens[1].empty()) //if not enough args
	{
		E461(user->getFd(), server->getHost(), user->getNick(), "KICK");
		return;
	}

	channel = server->findChannel(tokens[1]);
	if (channel == NULL)
	{
		E403(user->getFd(), server->getHost(), user->getNick(), tokens[1]);
		return;
	}
	if (channel->findUser(user->getFd()) == NULL && channel->findOperator(user->getFd()) == NULL)
	{
		E442(user->getFd(), server->getHost(), user->getNick(), tokens[1]);
		return;
	}
	if (channel->findOperator(user->getFd()) == NULL)
	{
		E482(user->getFd(), server->getHost(), user->getNick(), tokens[1]);
		return;
	}

	target = channel->findUser(tokens[2]);
	if (target == NULL)
	{
		E441(user->getFd(), server->getHost(), user->getNick(), tokens[1], tokens[2]);
		return;
	}
	
	if (channel->findUser(target->getFd()) != NULL)
	{
		userlist = channel->getUserList();
		for (std::deque<User*>::iterator it = userlist.begin(); it < userlist.end() ;it++)
		{
			if (tokens.size() > 3)
				RKICK((*it)->getFd(), server->getHost(), user->getNick(), tokens[1], tokens[2], tokens[3]);
			else
				RKICK((*it)->getFd(), server->getHost(), user->getNick(), tokens[1], tokens[2], "");
		}
		channel->delOperator(target->getFd());
		channel->delUser(target->getFd());
	}
	
	if (channel->getNbUser() < 1)
		server->deleteChannel(channel->getName());
}