#include "ft_irc.hpp"

void	RINVITE(REP_ARG, const std::string &channel);
void	E461(REP_ARG, const std::string &cmd);
void	E401(REP_ARG, const std::string &target_nick);
void 	E403(REP_ARG, const std::string &channel);
void	E442(REP_ARG, const std::string &channel);
void	E482(REP_ARG, const std::string &channel);
void	E443(REP_ARG, const std::string &channel, const std::string &target_nick);
void	R341(REP_ARG, const std::string &channel, const std::string &target_nick);

void	invite(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string nickname = "";
    std::string channel_name = "";

    if (user->getLoggedIn() == false)
        return;
    if (tokens.size() >= 2)
        nickname = tokens[1];
    if (tokens.size() >= 3)
		channel_name = tokens[2];
    if (tokens.size() <= 1 || nickname.empty()) //if not enough args
		E461(user->getFd(), server->getHost(), user->getNick(), "INVITE");
    else if (!server->findUser(nickname)) //if user don't exist
		E401(user->getFd(), server->getHost(), user->getNick(), nickname);
    else if (channel_name.size() < 1 || (channel_name[0] != '#' && channel_name[0] != '&'))//if channel_name don't exist (no types # &)
		E403(user->getFd(), server->getHost(), user->getNick(), channel_name);
    else
    {
        Channel* channel = server->findChannel(channel_name);
        if (!channel) //if channel_name don't exist
            E403(user->getFd(), server->getHost(), user->getNick(), channel_name);
        if (!channel->findUser(user->getFd())) //if user is not on channel
            E442(user->getFd(), server->getHost(), user->getNick(), channel_name);
        else if (channel->getModes().find("i") != std::string::npos && !channel->findOperator(user->getFd())) //if user is not operator of the channel
            E482(user->getFd(), server->getHost(), user->getNick(), channel_name);
        else if (channel->findUser(nickname)) //if target is already on channel
            E443(user->getFd(), server->getHost(), user->getNick(), channel_name, nickname);
        else
        {
            User* user_name = server->findUser(nickname);
            if (channel->getModes().find("i"))
                channel->addInvited(user_name);
            R341(user->getFd(), server->getHost(), user->getNick(), channel_name, nickname);
            R341(user_name->getFd(), server->getHost(), user->getNick(), channel_name, nickname);
        }
    }
}