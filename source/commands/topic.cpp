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
    (void)server;
    (void)user;
    (void)tokens;
}