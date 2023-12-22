#include "ft_irc.hpp"

void displayMessage(User *user)
{
    std::string message = user->getMessage();

    std::cout << "[" << user->getFd() << "]";
    std::cout << GREEN;
    for(std::string::iterator it=message.begin(); it!=message.end(); it++)
    {
        if (*it == '\r')
            std::cout << "\\r";
        else if (*it == '\n')
            std::cout << "\\n";
        else
            std::cout << *it;
    }
    std::cout << RESET << std::endl;
}

void displayReply(User *user)
{
    std::string message = user->getReply();

	std::cout << "[" << user->getFd() << "]";
    std::cout << CYAN;
    for(std::string::iterator it=message.begin(); it!=message.end(); it++)
    {
        if (*it == '\r')
            std::cout << "\\r";
        else if (*it == '\n')
            std::cout << "\\n";
        else
            std::cout << *it;
    }
    std::cout << RESET << std::endl;
}

void    displayAllUsers(Server *server)
{
    const std::deque<User *> users = server->getUsers();

    std::cout << "###################################################USERS###################################################" << std::endl;
    std::cout << "FD\t\tHostname\tNickname\tUsername\tRealname\tPWD Status\tStatus" << std::endl;
    for (size_t i = 0; i < users.size(); i++)
    {
        std::cout << users[i]->getFd() << "\t\t";
        std::cout << users[i]->getHostName() << "\t";
        if (users[i]->getNick().length() > 12)
            std::cout << users[i]->getNick().substr(0, 12) << "...\t";
        else if (users[i]->getNick().length() > 7)
            std::cout << users[i]->getNick() << "\t";
        else
            std::cout << users[i]->getNick() << "\t\t";
        if (users[i]->getUsername().length() > 12)
            std::cout << users[i]->getUsername().substr(0 ,12) << "...\t";
        else if (users[i]->getUsername().length() > 7)
            std::cout << users[i]->getUsername() << "\t";
        else
            std::cout << users[i]->getUsername() << "\t\t";
        if (users[i]->getRealname().length() > 12)
            std::cout << users[i]->getRealname().substr(0 ,12) << "...\t";
        else if (users[i]->getRealname().length() > 7)
            std::cout << users[i]->getRealname() << "\t";
        else
            std::cout << users[i]->getRealname() << "\t\t";
        std::cout << users[i]->getPass() << "\t\t";
        std::cout << users[i]->getLoggedIn() << "\t\t";
        //displayMessage(users[i]);
        std::cout << std::endl;
    }
}

void    displayAllChannels(Server *server)
{
    const std::deque<Channel *> channels = server->getChannels();

    std::cout << "###################################################CHANNELS###################################################" << std::endl;
	std::cout << "Name\t\tKey\t\tTopic\t\tModes\t\tnb_user_limit\tnb_user\t\tUserlist" << std::endl;
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cout << channels[i]->getName() << "\t\t";
		std::cout << channels[i]->getKey() << "\t\t";
		std::cout << channels[i]->getTopic() << "\t\t";
		std::cout << channels[i]->getModes() << "\t\t";
		std::cout << channels[i]->getNbUserLimit() << "\t\t";
		std::cout << channels[i]->getNbUser() << "\t\t";
		for (size_t j = 0; j < channels[i]->getUserList().size(); j++)
			std::cout << channels[i]->getUserList()[j]->getNick() << ", ";
		std::cout << std::endl;
    }
}

void	displayTokens(User *user)
{
    std::deque<std::string> tokens = user->getTokens();
	std::cout << "nb tokens: " << tokens.size() << std::endl;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		std::cout << "token " << i << ": ";
		std::cout << tokens[i] << std::endl;
	}
}