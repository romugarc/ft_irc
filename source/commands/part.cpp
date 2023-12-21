#include "ft_irc.hpp"

void	RPART(REP_ARG, const std::string &channel, const std::string &comment);

void	E461(REP_ARG, const std::string &cmd);
void 	E403(REP_ARG, const std::string &channel);
void	E442(REP_ARG, const std::string &channel);

void	part(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string channel_name = "";
    std::string reason = "";
    Channel* channel = NULL;

    if (user->getLoggedIn() == false)
        return;
    if (tokens.size() >= 2)
        channel_name = tokens[1];
    if (tokens.size() >= 3)
        reason = tokens[2];
    if (channel_name.size() > 0)
		channel = server->findChannel(channel_name);

    if (tokens.size() <= 1 || channel_name.empty()) //if not enough args
		E461(user->getFd(), server->getHost(), user->getNick(), "PART");
	else if (channel_name.size() < 1 || (channel_name[0] != '#' && channel_name[0] != '&') || !channel)//if channel_name don't exist (no types # &)
		E403(user->getFd(), server->getHost(), user->getNick(), channel_name);
    else if (!channel->findUser(user->getFd())) //if user is not on channel
        E442(user->getFd(), server->getHost(), user->getNick(), channel_name);
    else
    {
        std::deque<User*> chan_users = channel->getUserList();
        for (std::deque<User*>::iterator it = chan_users.begin(); it < chan_users.end() ;it++)
            RPART((*it)->getFd(), server->getHost(), user->getNick(), channel_name, reason);
        channel->delOperator(user->getFd());
	    channel->delUser(user->getFd());
        if (channel->getNbUser() < 1)
	        server->deleteChannel(channel->getName());
    }
}