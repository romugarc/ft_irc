#include "ft_irc.hpp"

void	RJOIN(User *u, Channel *c);
void	E461(const std::string &host, User *u, const std::string &cmd);
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void	E405(const std::string &host, User *u, Channel *c);
void	E475(const std::string &host, User *u, Channel *c);
void	E471(const std::string &host, User *u, Channel *c);
void	E473(const std::string &host, User *u, Channel *c);
void	R332(const std::string &host, User *u, Channel *c);
void	R353(const std::string &host, User *u1, Channel *c, User *u2, const char &symbol, const std::string &prefix);
void	R366(const std::string &host, User *u, Channel *c);

static bool	isOp(int user_fd, Channel* channel)
{
	std::deque<User*> op_users = channel->getOpList();

    for (size_t i = 0; i < op_users.size(); i++)
    {
        if (user_fd == op_users[i]->getFd())
            return (true);
    }
    return (false);
}

void	join(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string channel_name = "";
	std::string key = "";
	Channel* channel = NULL;

	if (user->getLoggedIn() == false)
        return;
    if (tokens.size() == 2)
        channel_name = tokens[1];
    else if (tokens.size() > 2)
	{
		channel_name = tokens[1];
		key = tokens[2];
	}
	if (channel_name.size() > 0)
		channel = server->findChannel(channel_name);

    if (tokens.size() <= 1 || channel_name.empty()) //if not enough args
		E461(server->getHost(), user, "JOIN");
	else if (channel_name.size() < 1 || (channel_name[0] != '#' && channel_name[0] != '&'))//if channel_name don't exist (no types # &)
		E403(server->getHost(), user, channel_name);
	else if (channel)//channel exist
	{
		if (user->getNbChanLimit() != 0 && user->getNbChan() >= user->getNbChanLimit()) //you have joined too many channels
			E405(server->getHost(), user, channel);
		else if (channel->getModes().find("k") != std::string::npos && key.compare(channel->getKey())) //+k mode -> channel_name requires a key and incorrect key
			E475(server->getHost(), user, channel);
		else if (channel->getModes().find("l") != std::string::npos && channel->getNbUserLimit() != 0 && channel->getNbUser() >= channel->getNbUserLimit()) //+l mode -> maximum limit of user already joined channel_name
			E471(server->getHost(), user, channel);
		else if (channel->getModes().find("i") != std::string::npos && !channel->findInvited(user->getFd()))//+i mode -> invite only and not invited to the channel_name
			E473(server->getHost(), user, channel);
		else //no error
		{
			channel->addUser(user);
			channel->delInvited(user->getFd());
			std::deque<User*> chan_users = channel->getUserList();
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
				RJOIN((*it), user, channel);
			if (!channel->getTopic().empty())//if topic exist (send the current topic of the channel_name)
				R332(server->getHost(), user, channel);
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
			{
				if (isOp((*it)->getFd(), channel))
					R353(server->getHost(), user, channel, (*it), '=', "@");
				else
					R353(server->getHost(), user, channel, (*it), '=', "");
			}
			R366(server->getHost(), user, channel); //end of the list R353
		}
	}
	else //channel don't exist
	{
		server->createChannel(user, channel_name);
		channel = server->findChannel(channel_name);
		RJOIN(user, user, channel);
		R353(server->getHost(), user, channel, user, '=', "@");
		R366(server->getHost(), user, channel);
	}
}
