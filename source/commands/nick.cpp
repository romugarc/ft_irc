#include "ft_irc.hpp"

void	E431(REP_ARG);
void	E432(REP_ARG, const std::string &nick);
void	E433(REP_ARG, const std::string &nick);
//void    E436(REP_ARG, const std::string &nick, const std::string &user, const std::string &host);

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

static bool isAlreadyUsed(Server *server, int user_fd, std::string nick)
{
    const std::deque<User *> users = server->getUsers();
    for (std::deque<User *>::const_iterator it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getFd() != user_fd && (*it)->getNick() == nick) //gerer les majuscules avec un toupper
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
        E431(user->getFd(), server->getHost(), user->getNick());
    else if (invalidCharacters(nick)) //if invalid character used
        E432(user->getFd(), server->getHost(), user->getNick(), nick);
    else if (isAlreadyUsed(server, user->getFd(), nick)) //if nickname already exist
        E432(user->getFd(), server->getHost(), user->getNick(), nick);
    else
    {
        //reussite
        user->setNick(nick);
    }
}