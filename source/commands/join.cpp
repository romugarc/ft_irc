#include "ft_irc.hpp"

void	RJOIN(REP_ARG, const std::string &channel);
void	E461(REP_ARG, const std::string &cmd);
void 	E403(REP_ARG, const std::string &channel);
void	E405(REP_ARG, const std::string &channel);
void	E475(REP_ARG, const std::string &channel);
//void	E474(REP_ARG, const std::string &channel); //err ban mode
void	E471(REP_ARG, const std::string &channel);
void	E473(REP_ARG, const std::string &channel);
//void	E476(REP_ARG, const std::string &channel);
void	R332(REP_ARG, const std::string &channel, const std::string &topic);
//void	R333(REP_ARG, const std::string &channel, const std::string &nick, const std::string &setat);
void	R353(REP_ARG, const char &symbol, const std::string &channel, const std::string &members);
void	R366(REP_ARG, const std::string &channel);

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
		E461(user->getFd(), server->getHost(), user->getNick(), "JOIN");
	else if (channel_name.size() < 1 || (channel_name[0] != '#' && channel_name[0] != '&'))//if channel_name don't exist (no types # &)
		E403(user->getFd(), server->getHost(), user->getNick(), channel_name);
	else if (channel)//channel exist
	{
		if (user->getNbChanLimit() != 0 && user->getNbChan() >= user->getNbChanLimit()) //you have joined too many channels
			E405(user->getFd(), server->getHost(), user->getNick(), channel->getName());
		else if (channel->getModes().find("k") != std::string::npos && key.compare(channel->getKey())) //+k mode -> channel_name requires a key and incorrect key
			E475(user->getFd(), server->getHost(), user->getNick(), channel->getName());
		else if (channel->getModes().find("l") != std::string::npos && channel->getNbUserLimit() != 0 && channel->getNbUser() >= channel->getNbUserLimit()) //+l mode -> maximum limit of user already joined channel_name
			E471(user->getFd(), server->getHost(), user->getNick(), channel->getName());
		else if (channel->getModes().find("i") != std::string::npos && !channel->findInvited(user->getFd()))//+i mode -> invite only and not invited to the channel_name
			E473(user->getFd(), server->getHost(), user->getNick(), channel->getName());
		else //no error
		{
			channel->addUser(user);
			channel->delInvited(user->getFd());
			std::deque<User*> chan_users = channel->getUserList();
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
				RJOIN((*it)->getFd(), server->getHost(), user->getNick(), channel->getName());
			if (!channel->getTopic().empty())//if topic exist (send the current topic of the channel_name)
				R332(user->getFd(), server->getHost(), user->getNick(), channel->getName(), channel->getTopic());
			//commande NAMES
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
			{
				if (isOp((*it)->getFd(), channel))
					R353(user->getFd(), server->getHost(), user->getNick(), '=', channel->getName(), "@", (*it)->getNick());
				else
					R353(user->getFd(), server->getHost(), user->getNick(), '=', channel->getName(), "", (*it)->getNick());
			}
			R366(user->getFd(), server->getHost(), user->getNick(), channel->getName()); //end of the list R353
		}
	}
	else //channel don't exist
	{
		server->createChannel(user, channel_name);
		RJOIN(user->getFd(), server->getHost(), user->getNick(), channel_name);
		R353(user->getFd(), server->getHost(), user->getNick(), '=', channel_name, "@", user->getNick());
		R366(user->getFd(), server->getHost(), user->getNick(), channel_name);
	}
}
