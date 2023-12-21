#ifndef REP_HPP
#define	REP_HPP

#include "ft_irc.hpp"

#define REP_ARG int const &fd, const std::string &host, const std::string &client

void	send_to_client(std::string msg, int const &fd, Server *server);

void	RJOIN(REP_ARG, const std::string &channel);
void	RMODE(REP_ARG, const std::string &target, const char operation, const char mode, const std::string &param);
void	RKICK(REP_ARG, const std::string &channel, const std::string &nick, const std::string &comment);
void	RQUIT(REP_ARG, const std::string &comment);
void	RPRIVMSG(REP_ARG, const std::string &target, const std::string &content);

void 	R001(REP_ARG, const std::string &user, const std::string &userhost);
void	R221(REP_ARG, const std::string &user_modes);
void	R324(REP_ARG, const std::string &channel, const std::string &mode, const std::string &param_lst);
void	R331(REP_ARG, const std::string &channel);
void	R332(REP_ARG, const std::string &channel, const std::string &topic);
void	R341(REP_ARG, const std::string &channel, const std::string &input_name);
void    R353(REP_ARG, const char &symbol, const std::string &channel, const std::string &prefix, const std::string &nick);
void	R366(REP_ARG, const std::string &channel);
void	R372(REP_ARG, const std::string &line);
void	R375(REP_ARG);
void	R376(REP_ARG);
void	R391(REP_ARG);

void	E401(REP_ARG, const std::string &input_name);
void	E402(REP_ARG, const std::string &server_name);
void 	E403(REP_ARG, const std::string &channel);
void 	E404(REP_ARG, const std::string &channel);
void	E405(REP_ARG, const std::string &channel);
void	E411(REP_ARG, const std::string &cmd);
void	E412(REP_ARG);
void    E421(REP_ARG, const std::string &cmd);
void    E422(REP_ARG);
void 	E431(REP_ARG);
void	E432(REP_ARG, const std::string &bad_name);
void	E433(REP_ARG, const std::string &bad_name);
void	E441(REP_ARG, const std::string &channel, const std::string &input_name);
void	E442(REP_ARG, const std::string &channel);
void	E443(REP_ARG, const std::string &channel, const std::string &input_name);
void	E461(REP_ARG, const std::string &cmd);
void	E462(REP_ARG);
void	E464(REP_ARG);
void	E471(REP_ARG, const std::string &channel);
void 	E472(REP_ARG, const std::string &channel, const char &mode);
void	E473(REP_ARG, const std::string &channel);
void	E475(REP_ARG, const std::string &channel);
void	E482(REP_ARG, const std::string &channel);
void	E501(REP_ARG);
void	E502(REP_ARG);
void    E525(REP_ARG, const std::string &channel);
void    E696(REP_ARG, const std::string &channel, const std::string &mode, const std::string &password);


#endif