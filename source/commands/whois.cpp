#include "ft_irc.hpp"

void	E431(const std::string &host, User *u);
void	E401(const std::string &host, User *u, const std::string &target_nick);
void	R311(const std::string &host, User *u1, User *u2);
void	R318(const std::string &host, User *u);

void	whois(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string nick = "";

    if (user->getLoggedIn() == false)
        return;
    if (tokens.size() > 1)
        nick = tokens[1];
    if (tokens.size() <= 1 || nick.empty()) //if not enough args
        E431(server->getHost(), user);
    else if (!server->findUser(nick)) //if user don't exist
		E401(server->getHost(), user, nick);
    else
    {
        std::deque<User *>users =server->getUsers();
        for (std::deque<User *>::iterator it = users.begin(); it != users.end(); ++it)
        {
            if (tokens[1] == (*it)->getNick())
                R311(server->getHost(), user, (*it));
        }
        R318(server->getHost(), user);
    }
}