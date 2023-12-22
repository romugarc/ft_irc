#ifndef REP_HPP
#define	REP_HPP

#include "ft_irc.hpp"

void	send_to_client(User *u);

void	RNICK(User *u1, User *u2, const std::string &nick);
void	RJOIN(User *u1, User *u2, Channel *c);
void	RPART(User *u1, User *u2, Channel *c, const std::string &comment);
void	RKICK(User *u1, User *u2, Channel *c, const std::string &nick, const std::string &comment);
void	RMODE(User *u1, User *u2, const std::string &target, const char operation, const char mode, const std::string &param);
void	RQUIT(User *u1, User *u2, const std::string &comment);
void	RPRIVMSG(User *u1, User *u2, const std::string &target, const std::string &content);

void 	R001(const std::string &host, User *u);
void	R221(const std::string &host, User *u);
void	R311(const std::string &host, User *u1, User *u2);
void	R318(const std::string &host, User *u);
void	R319(const std::string &host, User *u, Channel *c);
void	R324(const std::string &host, User *u, Channel *c);
void	R331(const std::string &host, User *u, Channel *c);
void	R332(const std::string &host, User *u, Channel *c);
void	R341(const std::string &host, User *u1, Channel *c, User *u2);
void	R353(const std::string &host, User *u1, Channel *c, User *u2, const char &symbol, const std::string &prefix);
void	R366(const std::string &host, User *u, Channel *c);

void	E401(const std::string &host, User *u, const std::string &target_nick);
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void 	E404(const std::string &host, User *u, Channel *c);
void	E405(const std::string &host, User *u, Channel *c);
void	E411(const std::string &host, User *u, const std::string &cmd);
void	E412(const std::string &host, User *u);
void	E421(const std::string &host, User *u);
void	E431(const std::string &host, User *u);
void	E432(const std::string &host, User *u, const std::string &nick);
void	E433(const std::string &host, User *u, const std::string &nick);
void	E441(const std::string &host, User *u, Channel *c, const std::string &target_nick);
void	E442(const std::string &host, User *u, Channel *c);
void	E443(const std::string &host, User *u1, Channel *c, User *u2);
void	E461(const std::string &host, User *u, const std::string &cmd);
void	E462(const std::string &host, User *u);
void	E464(const std::string &host, User *u);
void	E471(const std::string &host, User *u, Channel *c);
void 	E472(const std::string &host, User *u, Channel *c, const char &mode);
void	E473(const std::string &host, User *u, Channel *c);
void	E475(const std::string &host, User *u, Channel *c);
void	E482(const std::string &host, User *u, Channel *c);
void	E501(const std::string &host, User *u);
void	E502(const std::string &host, User *u);
void	E525(const std::string &host, User *u, Channel *c);

#endif