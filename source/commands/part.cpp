#include "ft_irc.hpp"

void	RPART(User *u1, User *u2, Channel *c, const std::string &comment);
void	E461(const std::string &host, User *u, const std::string &cmd);
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void	E442(const std::string &host, User *u, Channel *c);

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
		E461(server->getHost(), user, "PART");
	else if (channel_name.size() < 1 || (channel_name[0] != '#') || !channel)//if channel_name don't exist (no types # &)
		E403(server->getHost(), user, channel_name);
    else if (!channel->findUser(user->getFd())) //if user is not on channel
        E442(server->getHost(), user, channel);
    else
    {
        std::deque<User*> chan_users = channel->getUserList();
        for (std::deque<User*>::iterator it = chan_users.begin(); it < chan_users.end() ;it++)
            RPART((*it), user, channel, reason);
        channel->delOperator(user->getFd());
	    channel->delUser(user->getFd());
        if (channel->getNbUser() < 1)
	        server->deleteChannel(channel->getName());
    }
}