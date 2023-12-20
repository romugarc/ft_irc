#include "ft_irc.hpp"

void	RMODE(REP_ARG, const std::string &target, const char operation, const char mode);

void	E401(REP_ARG, const std::string &input_name);
void	E502(REP_ARG);
void	R221(REP_ARG, const std::string &user_modes);
void    E501(REP_ARG);

void 	E403(REP_ARG, const std::string &channel);
void	R324(REP_ARG, const std::string &channel, const std::string &mode);
//329 date de creation pas oblige
void	E482(REP_ARG, const std::string &channel);

//ban +b
//367
//368

//exception +e
//348
//349

//invite-exception +I
//336
//337

static bool check_user_mode(std::string mode)
{
    if (mode.size() == 2 && (mode[0] == '+' || mode[0] == '-'))
    {
        if (mode[1] == 'B' || mode[1] == 'd' || mode[1] == 'D'
            || mode[1] == 'G' || mode[1] == 'H' || mode[1] == 'I'
            || mode[1] == 'i' || mode[1] == 'o' || mode[1] == 'p' || mode[1] == 'q'
            || mode[1] == 'r' || mode[1] == 'R' || mode[1] == 'S' || mode[1] == 's'
            || mode[1] == 'T' || mode[1] == 't' || mode[1] == 'W' || mode[1] == 'w'
            || mode[1] == 'x' || mode[1] == 'Z' || mode[1] == 'z')
            return true;
    }
    else if (mode.size() == 1 && (mode[0] == 'B' || mode[0] == 'd'
        || mode[0] == 'D' || mode[0] == 'G' || mode[0] == 'H' || mode[0] == 'I'
        || mode[0] == 'i' || mode[0] == 'o' || mode[0] == 'p' || mode[0] == 'q'
        || mode[0] == 'r' || mode[0] == 'R' || mode[0] == 'S' || mode[0] == 's'
        || mode[0] == 'T' || mode[0] == 't' || mode[0] == 'W' || mode[0] == 'w'
        || mode[0] == 'x' || mode[0] == 'Z' || mode[0] == 'z'))
        return true;
    return false;
}

static bool check_channel_mode(std::string mode)
{
    if (mode.size() == 2 && (mode[0] == '+' || mode[0] == '-'))
    {
        if (mode[1] == 'i' || mode[1] == 't' || mode[1] == 'k'
            || mode[1] == 'o' || mode[1] == 'l')
            return true;
    }
    else if (mode.size() == 1 && (mode[0] == 'i' || mode[0] == 't'
        || mode[0] == 'k' || mode[0] == 'o' || mode[0] == 'l'))
        return true;
    return false;
}

//i: invite-only   t: topic restriction   k: key   o: give operator privilege   l: user limit
void	mode(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string target = "";
    std::string mode = "";
    std::string param = "";

    if (tokens.size() >= 2)
        target = tokens[1];
    if (tokens.size() >= 3)
		mode = tokens[2];
    if (tokens.size() >= 4)
		param = tokens[3];

    if (target.empty())
        E461(user->getFd(), server->getHost(), user->getNick(), "MODE");
    else if (target.size() >= 1 && (target[0] != '#' && target[0] != '&'))
    {
        //user mode
        if (!server->findUser(target))
            E401(user->getFd(), server->getHost(), user->getNick(), target);
        else if (user->getNick().compare(target))
            E502(user->getFd(), server->getHost(), user->getNick());
        else if (mode.empty())
            R221(user->getFd(), server->getHost(), user->getNick(), user->getModes());
        else
        {
            if (!check_user_mode(mode))
                E501(user->getFd(), server->getHost(), user->getNick());
            else
            {
                //ajouter ou supprimer mode de user pas a gerer je crois et on ignore les params
                if (mode.size() == 2)
                    RMODE(user->getFd(), server->getHost(), user->getNick(), target, mode[0], mode[1]);
                else if (mode.size() == 1)
                    RMODE(user->getFd(), server->getHost(), user->getNick(), target, '+', mode[0]);
            }
        }
    }
    else
    {
        //channel mode
        if (!server->findChannel(target))
            E403(user->getFd(), server->getHost(), user->getNick(), target);
        else if (mode.empty())
            R324(user->getFd(), server->getHost(), user->getNick(), target, user->getModes()); //param pas a gerer
        else
        {
            Channel* channel = server->findChannel(target);
            if (!channel->findOperator(user->getFd())) //utilisateur n'es pas un operateur
                E482(user->getFd(), server->getHost(), user->getNick(), target);
            else if (!check_channel_mode(mode))
            {
                //ajouter ou supprimer mode de channel
                if (mode.size() == 2)
                    RMODE(user->getFd(), server->getHost(), user->getNick(), target, mode[0], mode[1]);
                else if (mode.size() == 1)
                    RMODE(user->getFd(), server->getHost(), user->getNick(), target, '+', mode[0]);
            }
        }
    }
}