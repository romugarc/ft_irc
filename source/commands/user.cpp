#include "ft_irc.hpp"

void	E461(REP_ARG, const std::string &cmd);
void	E462(REP_ARG);

void	user(Server *server, User *user, std::deque<std::string> tokens)
{
  (void)server;
  if (tokens.size() <= 1 || tokens[1].empty()) //if not enough args
		E461(user->getFd(), user->getNick(), "USER");
	else if (user->getLoggedIn() == true) //if registration already done
		E462(user->getFd(), user->getNick());
  else
  {
    //reussite
    user->setUsername(tokens[1]);
  }
  //set the <realname> ?
}