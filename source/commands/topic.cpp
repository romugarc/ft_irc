#include "ft_irc.hpp"

void	E461(const std::string &host, User *u, const std::string &cmd);
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void	E442(const std::string &host, User *u, Channel *c);
void	E482(const std::string &host, User *u, Channel *c);
void	R331(const std::string &host, User *u, Channel *c);
void	R332(const std::string &host, User *u, Channel *c);

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
		E461(server->getHost(), user, "TOPIC");
    else if (channel_name.size() < 1 || (channel_name[0] != '#'))//if channel_name don't exist (no types # &)
		E403(server->getHost(), user, channel_name);
    else
    {
        Channel* channel = server->findChannel(channel_name);
        if (!channel)//if channel_name don't exist
            E403(server->getHost(), user, channel_name);
        else if (!channel->findUser(user->getFd())) //if user is not on channel
            E442(server->getHost(), user, channel);
        else if (tokens.size() <= 2 || topic.empty()) //if no topic arg
        {
            if (channel->getTopic().empty()) //if no topic in channel
                R331(server->getHost(), user, channel);
            else //get topic
                R332(server->getHost(), user, channel);
        }
        else if (channel->getModes().find("t") != std::string::npos && !channel->findOperator(user->getFd())) //if user is not operator of the channel
            E482(server->getHost(), user, channel);
        else //set topic
        {
            std::deque<User*> chan_users = channel->getUserList();
            for (std::deque<User*>::iterator it = chan_users.begin(); it < chan_users.end() ;it++)
                RTOPIC((*it), user, channel, topic);
            channel->setTopic(topic);
        }
    }
}