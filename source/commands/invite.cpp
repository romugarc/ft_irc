#include "ft_irc.hpp"

void	RINVITE(User *u1, User *u2, Channel *c);
void	E461(const std::string &host, User *u, const std::string &cmd);
void	E401(const std::string &host, User *u, const std::string &target_nick);
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void	E442(const std::string &host, User *u, Channel *c);
void	E482(const std::string &host, User *u, Channel *c);
void	E443(const std::string &host, User *u1, Channel *c, User *u2);
void	R341(const std::string &host, User *u1, Channel *c, User *u2);

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
		E461(server->getHost(), user, "INVITE");
    else if (!server->findUser(nickname)) //if user don't exist
		E401(server->getHost(), user, nickname);
    else if (channel_name.size() < 1 || (channel_name[0] != '#' && channel_name[0] != '&'))//if channel_name don't exist (no types # &)
		E403(server->getHost(), user, channel_name);
    else
    {
        Channel* channel = server->findChannel(channel_name);
        if (!channel) //if channel_name don't exist
            E403(server->getHost(), user, channel_name);
        else if (!channel->findUser(user->getFd())) //if user is not on channel
            E442(server->getHost(), user, channel);
        else if (channel->getModes().find("i") != std::string::npos && !channel->findOperator(user->getFd())) //if user is not operator of the channel
            E482(server->getHost(), user, channel);
        else
        {
            User* invited_user = channel->findUser(nickname);
            if (invited_user) //if target is already on channel
                E443(server->getHost(), user, channel, invited_user);
            else
            {
                User* invited_user = server->findUser(nickname);
                if (channel->getModes().find("i"))
                    channel->addInvited(invited_user);
                R341(server->getHost(), user, channel, invited_user);
                RINVITE(invited_user, user, channel);
            }
        }
    }
}