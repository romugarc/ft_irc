#include "ft_irc.hpp"

void	E461(const std::string &host, User *u, const std::string &cmd);
void	E462(const std::string &host, User *u);

void	user(Server *server, User *user, std::deque<std::string> tokens)
{
  (void)server;

  if (user->getPass() == false)
    return;
  if (tokens.size() <= 1 || tokens[1].empty()) //if not enough args
		E461(server->getHost(), user, "USER");
	else if (user->getLoggedIn() == true) //if registration already done
		E462(server->getHost(), user);
  else
  {
    user->setUsername(tokens[1]);
    //realname
    if (tokens.size() >= 5 && !tokens[2].compare("0") && !tokens[3].compare("*"))
      user->setRealname(tokens[4]);
  }
}