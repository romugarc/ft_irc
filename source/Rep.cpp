#include "Rep.hpp"

void	RNICK(User *u1, User *u2, const std::string &nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << u2->getClientId() << " NICK " << nick << "\r\n";
	u1->addReply(output.str());
}

void	RINVITE(User *u1, User *u2, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << u2->getClientId() << " INVITE " << u1->getNick() << " " << c->getName() << "\r\n";
	u1->addReply(output.str());
}

void	RJOIN(User *u1, User *u2, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << u2->getClientId() << " JOIN " << c->getName() << "\r\n";
	u1->addReply(output.str());
}

void	RPART(User *u1, User *u2, Channel *c, const std::string &comment)
{
	std::stringstream	output;

	output.str("");
	if (comment.size() > 0)
		output << ":" << u2->getClientId() << " PART " << c->getName() << " " << comment << "\r\n";
	else
		output << ":" << u2->getClientId() << " PART " << c->getName() << " " << "No comment" << "\r\n";
	u1->addReply(output.str());
}

void	RKICK(User *u1, User *u2, Channel *c, const std::string &nick, const std::string &comment)
{
	std::stringstream	output;

	output.str("");
	if (comment.size() > 0)
		output << ":" << u2->getClientId() << " KICK " << c->getName() << " " << nick << " :" << comment << "\r\n";
	else
		output << ":" << u2->getClientId() << " KICK " << c->getName() << " " << nick << " :No comment" << "\r\n";
	u1->addReply(output.str());
}

void	RMODE(User *u1, User *u2, const std::string &target, const char operation, const char mode, const std::string &param)
{
	std::stringstream	output;

	output.str("");
	output << ":" << u2->getClientId() << " MODE " << target << " " << operation << mode << " " << param << "\r\n";
	u1->addReply(output.str());
}

void	RQUIT(User *u1, User *u2, const std::string &comment)
{
	std::stringstream	output;

	output.str("");
	output << ":" << u2->getClientId() << " QUIT " << ":Quit: " << comment << "\r\n";
	u1->addReply(output.str());
}

void	RPRIVMSG(User *u1, User *u2, const std::string &target, const std::string &content)
{
	std::stringstream	output;

	output.str("");
	output << ":" << u2->getClientId() << " PRIVMSG " << target << " :" << content << "\r\n";
	u1->addReply(output.str());
}

void 	R001(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 001 " << u->getNick() << " :Welcome to the IRC 42 Network, " << u->getClientId() << "\r\n";
	u->addReply(output.str());
}

void	R221(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 221 " << u->getNick() << " +" << u->getModes() << "\r\n";
	u->addReply(output.str());
}

void	R311(const std::string &host, User *u1, User *u2)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 311 " << u1->getNick() << " " << u2->getNick() << " " << u2->getUsername() << " " << u2->getHostName() << " * :" << u2->getRealname() << "\r\n";
	u1->addReply(output.str());
}

void	R318(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 318 " << u->getNick() << " " << u->getNick() << " :End of /WHOIS list" << "\r\n";
	u->addReply(output.str());
}

void	R324(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 324 " << u->getNick() << " " << c->getName() << " +" << c->getModes() << " " << c->getKey();
	if (c->getNbUserLimit() > 0)
        output << " " << c->getNbUserLimit();
	output << "\r\n";
	u->addReply(output.str());
}

void	R331(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 331 " << u->getNick() << " " << c->getName() << " :No topic is set" << "\r\n";
	u->addReply(output.str());
}

void	R332(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 332 " << u->getNick() << " " << c->getName() << " :" << c->getTopic() << "\r\n";
	u->addReply(output.str());
}

void	R341(const std::string &host, User *u1, Channel *c, User *u2)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 341 " << u1->getNick() << " " << c->getName() << " " << u2->getNick() << "\r\n";
	u1->addReply(output.str());
}

void	R353(const std::string &host, User *u1, Channel *c, User *u2, const char &symbol, const std::string &prefix)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 353 " << u1->getNick() << " " << symbol << " " << c->getName() << " :" << prefix << u2->getNick() << "\r\n";
	u1->addReply(output.str());
}

void	R366(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 366 " << u->getNick() << " " << c->getName() << " :End of /NAMES list" << "\r\n";
	u->addReply(output.str());
}

void	E401(const std::string &host, User *u, const std::string &target_nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 401 " << u->getNick() << " " << target_nick << " :No such nick/channel" << "\r\n";
	u->addReply(output.str());
}

void 	E403(const std::string &host, User *u, const std::string &target_channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 403 " << u->getNick() << " " << target_channel << " :No such channel" << "\r\n";
	u->addReply(output.str());
}

void 	E404(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 404 " << u->getNick() << " " << c->getName() << " :Cannot send to channel" << "\r\n";
	u->addReply(output.str());
}

void	E405(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 405 " << u->getNick() << " " << c->getName() <<  " :You have joined too many channels" << "\r\n";	
	u->addReply(output.str());
}

void	E411(const std::string &host, User *u, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 411 " << u->getNick() << " :No recipient given (" << cmd << ")" << "\r\n";
	u->addReply(output.str());
}

void	E412(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 412 " << u->getNick() << " :No text to send" << "\r\n";
	u->addReply(output.str());
}

void	E421(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 421 " << u->getNick() << " " << u->getTokens()[0] << " :Unknown command" << "\r\n";
	u->addReply(output.str());
}

void	E431(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 431 " << u->getNick() << " :No nickname given" << "\r\n";
	u->addReply(output.str());
}

void	E432(const std::string &host, User *u, const std::string &nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 432 " << u->getNick() << " " << nick << " :Erroneous nickname" << "\r\n";
	u->addReply(output.str());
}

void	E433(const std::string &host, User *u, const std::string &nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 433 " << u->getNick() << " " << nick << " :Nickname is already in use" << "\r\n";
	u->addReply(output.str());
}

void	E441(const std::string &host, User *u, Channel *c, const std::string &target_nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 441 " << u->getNick() << " " << target_nick << " " << c->getName() << " :They aren't on that channel" << "\r\n";
	u->addReply(output.str());
}

void	E442(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 442 " << u->getNick() << " " << c->getName() << " :You're not on that channel" << "\r\n";
	u->addReply(output.str());
}

void	E443(const std::string &host, User *u1, Channel *c, User *u2)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 443 " << u1->getNick() << " " << u2->getNick() << " " << c->getName() << " :is already on channel" << "\r\n";
	u1->addReply(output.str());
}

void	E461(const std::string &host, User *u, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 461 " << u->getNick() << " " << cmd << " :Not enough parameters" << "\r\n";
	u->addReply(output.str());
}

void	E462(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 462 " << u->getNick() << " :Unauthorized command (already registered)" << "\r\n";
	u->addReply(output.str());
}

void	E464(const std::string &host, User *u)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 464 " << u->getNick() << " :Password incorrect" << "\r\n";
	u->addReply(output.str());
}

void	E471(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 471 " << u->getNick() << " " << c->getName() << " :Cannot join channel (+l)" << "\r\n";
	u->addReply(output.str());
}

void 	E472(const std::string &host, User *u, Channel *c, const char &mode) //mode user
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 472 " << u->getNick() << " " << mode << " :is unknown mode char to me for " << c->getName() << "\r\n";
	u->addReply(output.str());
}

void	E473(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 473 " << u->getNick() << " " << c->getName() << " :Cannot join channel (+i)" << "\r\n";
	u->addReply(output.str());
}

void	E475(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 475 " << u->getNick() << " " << c->getName() << " :Cannot join channel (+k)" << "\r\n";
	u->addReply(output.str());
}

void	E482(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 482 " << u->getNick() << " " << c->getName() << " :You're not channel operator" << "\r\n";
	u->addReply(output.str());
}

void	E501(const std::string &host, User *u) //mode user
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 501 " << u->getNick() << " :Unknown MODE flag" << "\r\n";
	u->addReply(output.str());
}

void	E502(const std::string &host, User *u) //mode user
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 502 " << u->getNick() << " :Cant change mode for other users" << "\r\n";
	u->addReply(output.str());
}

void	E525(const std::string &host, User *u, Channel *c)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 525 " << u->getNick() << " " << c->getName() << " :Key is not well-formed" << "\r\n";
	u->addReply(output.str());
}

