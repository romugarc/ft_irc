#include "ft_irc.hpp"

void	RMODE(User *u1, User *u2, const std::string &target, const char operation, const char mode, const std::string &param);
void	E461(const std::string &host, User *u, const std::string &cmd);
void	E401(const std::string &host, User *u, const std::string &target_nick);
void	E502(const std::string &host, User *u); //mode user
void	R221(const std::string &host, User *u);
void	E501(const std::string &host, User *u); //mode user
void 	E403(const std::string &host, User *u, const std::string &target_channel);
void	R324(const std::string &host, User *u, Channel *c);
void	E482(const std::string &host, User *u, Channel *c);
void	E441(const std::string &host, User *u, Channel *c, const std::string &target_nick);

static bool check_channel_mode(std::string mode)
{
    if (mode.size() == 2 && (mode[0] == '+' || mode[0] == '-'))
    {
        if (mode[1] == 'i' || mode[1] == 't' || mode[1] == 'k'
            || mode[1] == 'o' || mode[1] == 'l' || mode[1] == 'n')
            return true;
    }
    else if (mode.size() == 1 && (mode[0] == 'i' || mode[0] == 't'
        || mode[0] == 'k' || mode[0] == 'o' || mode[0] == 'l' || mode[0] == 'n'))
        return true;
    return false;
}

static void remove_mode(Server *server, User *user, Channel* channel, std::string target, std::string mode_str, std::string param)
{
    char mode = 0;

    if (mode_str.size() == 2)
        mode = mode_str[1];
    if (mode == 'i' || mode == 't' || mode == 'k' || mode == 'l' || mode == 'n')
    {
        channel->removeMode(mode);
        if (mode == 'k')
            channel->setKey("");
        else if (mode == 'l')
            channel->setNbUserLimit(0);
        std::deque<User*> chan_users = channel->getUserList();
		for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
            RMODE((*it), user, target, '-', mode, "");
    }
    else if (mode == 'o' && param.size())
    {
        User* user_target = channel->findUser(param);
        if (!user_target)
            E441(server->getHost(), user, channel, param);
        else
        {
            channel->delOperator(user_target->getFd());
            std::deque<User*> chan_users = channel->getUserList();
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
                RMODE((*it), user, target, '-', mode, param);
        }
    }
}

static bool checkkey(std::string param) 
{
    for (size_t i = 0; i < param.length(); i++) 
	{
        if (!isalpha(param[i]) && !isdigit(param[i])) 
            return (false);
    }
    return (true);
}

static int stoi(std::string str)
{
    int nbr = 0;
    int len = (int)str.size();

    for (unsigned int i = 0; i < (unsigned int)len; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            break;
        nbr *= 10;
        nbr += (str[i] - 48);
    };
    return nbr;
};

static void add_mode(Server *server, User *user, Channel* channel, std::string target, std::string mode_str, std::string param)
{
    char mode = 0;

    if (mode_str.size() == 1)
        mode = mode_str[0];
    else if (mode_str.size() == 2)
        mode = mode_str[1];
    if (mode == 'i' || mode == 't' || mode == 'n')
    {
        channel->addMode(mode);
        std::deque<User*> chan_users = channel->getUserList();
		for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
            RMODE((*it), user, target, '+', mode, "");
    }
    else if (mode == 'k' && param.size())
    {
        if (!checkkey(param))
            E525(server->getHost(), user, channel);
        else
        {
            channel->addMode(mode);
            channel->setKey(param);
            std::deque<User*> chan_users = channel->getUserList();
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
                RMODE((*it), user, target, '+', mode, param);
        }
    }
    else if (mode == 'l' && param.size())
    {
        int limit = stoi(param);
        std::stringstream sstm;
        sstm << limit;
        std::string limit_str = sstm.str();
 
        channel->setNbUserLimit(limit);
        if (limit > 0)
        {
            channel->addMode(mode);
            std::deque<User*> chan_users = channel->getUserList();
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
                RMODE((*it), user, target, '+', mode, limit_str);
        }
        else
        {
            channel->removeMode(mode);
            std::deque<User*> chan_users = channel->getUserList();
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
                RMODE((*it), user, target, '-', mode, "");
        }
    }
    else if (mode == 'o' && param.size())
    {
        User* user_target = channel->findUser(param);
        if (!user_target)
            E441(server->getHost(), user, channel, param);
        else
        {
            std::deque<User*> chan_users = channel->getUserList();
            channel->addOperator(user_target);
			for (std::deque<User *>::iterator it = chan_users.begin(); it < chan_users.end(); it++)
                RMODE((*it), user, target, '+', mode, param);
        }
    }
}

//i: invite-only   t: topic restriction   k: key   o: give operator privilege   l: user limit
void	mode(Server *server, User *user, std::deque<std::string> tokens)
{
    std::string target = "";
    std::string mode = "";
    std::string param = "";

    if (user->getLoggedIn() == false)
        return;
    if (tokens.size() >= 2)
        target = tokens[1];
    if (tokens.size() >= 3)
		mode = tokens[2];
    if (tokens.size() >= 4)
		param = tokens[3];

    if (target.empty())
        E461(server->getHost(), user, "MODE");
    else if (target.size() >= 1 && (target[0] != '#' && target[0] != '&'))
    {
        //user mode
        E501(server->getHost(), user);
    }
    else
    {
        //channel mode
        Channel* channel = server->findChannel(target);
        if (!channel)
            E403(server->getHost(), user, target);
        else
        {
            if (mode.empty())
                R324(server->getHost(), user, channel);
            else if (!channel->findOperator(user->getFd())) //utilisateur n'es pas un operateur
                E482(server->getHost(), user, channel);
            else if (!check_channel_mode(mode))
                E501(server->getHost(), user);
            else
            {
                if (mode[0] == '-')
                    remove_mode(server, user, channel, target, mode, param);
                else
                    add_mode(server, user, channel, target, mode, param);
            }
        }
    }
}

//i t pas besoin de param
//k param(key)
//l param(user_limit)
//o param(nick_user)