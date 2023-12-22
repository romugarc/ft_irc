#include "ft_irc.hpp"

void	RNICK(User *u, const std::string &nick);
void	E431(const std::string &host, User *u);
void	E432(const std::string &host, User *u, const std::string &nick);
void	E433(const std::string &host, User *u, const std::string &nick);

static bool invalidCharacters(std::string str)
{
    std::string    forbidden = "&,*?!@.#$:\"\'"; //ajouter les characteres interdits

    for (std::string::iterator it = forbidden.begin(); it != forbidden.end (); it++)
    {
        if (str.find(*it) != std::string::npos)
            return (true);
    }
    return (false);
}

static std::string str_toupper(std::string s)
{
    for (unsigned int i = 0; i < s.size(); i++)
        s[i] = (char)std::toupper(s[i]);
    return (s);
}

static bool isAlreadyUsed(Server *server, int user_fd, std::string nick)
{
    const std::deque<User *> users = server->getUsers();
    for (std::deque<User *>::const_iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getFd() != user_fd && str_toupper((*it)->getNick()) == str_toupper(nick))
            return (true);
    }
    return (false);
}

void	nick(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string nick = "";
    if (tokens.size() > 1)
        nick = tokens[1];
    if (tokens.size() <= 1 || nick.empty()) //if not enough args
        E431(server->getHost(), user);
    else if (invalidCharacters(nick)) //if invalid character used
        E432(server->getHost(), user, nick);
    else if (isAlreadyUsed(server, user->getFd(), nick)) //if nickname already exist
        E433(server->getHost(), user, nick);
    else
    {
        RNICK(user, user, nick);
        user->setNick(nick);
    }
}