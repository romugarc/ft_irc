#include "ft_irc.hpp"

//void	RTOPIC(REP_ARG, const std::string &channel);
void	E461(REP_ARG, const std::string &cmd);
void 	E403(REP_ARG, const std::string &channel);
void	E442(REP_ARG, const std::string &channel);
void	E482(REP_ARG, const std::string &channel);

void	R331(REP_ARG, const std::string &channel);
void	R332(REP_ARG, const std::string &channel, const std::string &topic);
//void	R333(REP_ARG, const std::string &channel, const std::string &nick, const std::string &setat);

void	topic(Server *server, User *user, std::deque<std::string> tokens)
{
	std::string	channel_name = "";
	std::string	topic = "";

    if (user->getLoggedIn() == false)
        return;
    if (tokens.size() >= 2)
        channel_name = tokens[1];
    if (tokens.size() >= 3)
		topic = tokens[2];
    if (tokens.size() <= 1 || channel_name.empty()) //if not enough args
		E461(user->getFd(), server->getHost(), user->getNick(), "TOPIC");
    else if (channel_name.size() < 1 || (channel_name[0] != '#' && channel_name[0] != '&'))//if channel_name don't exist (no types # &)
		E403(user->getFd(), server->getHost(), user->getNick(), channel_name);
    else
    {
        Channel* channel = server->findChannel(channel_name);
        if (!channel)//if channel_name don't exist
            E403(user->getFd(), server->getHost(), user->getNick(), channel_name);
        else if (!channel->findUser(user->getFd())) //if user is not on channel
            E442(user->getFd(), server->getHost(), user->getNick(), channel_name);
        else if (tokens.size() <= 2 || topic.empty()) //if no topic arg
        {
            if (channel->getTopic().empty()) //if no topic in channel
                R331(user->getFd(), server->getHost(), user->getNick(), channel_name);
            else //get topic
                R332(user->getFd(), server->getHost(), user->getNick(), channel_name, channel->getTopic());
        }
        else if (channel->getModes().find("t") != std::string::npos && !channel->findOperator(user->getFd())) //if user is not operator of the channel
            E482(user->getFd(), server->getHost(), user->getNick(), channel_name);
        else //set topic
        {
            channel->setTopic(topic);
            R332(user->getFd(), server->getHost(), user->getNick(), channel_name, channel->getTopic());
        }
    }
}