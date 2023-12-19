#include "ft_irc.hpp"

void	E461(REP_ARG, const std::string &cmd);
void 	E403(REP_ARG, const std::string &channel);
void	E405(REP_ARG, const std::string &channel);
void	E475(REP_ARG, const std::string &channel);
//void	E474(REP_ARG, const std::string &channel);
void	E471(REP_ARG, const std::string &channel);
void	E473(REP_ARG, const std::string &channel);
//void	E476(REP_ARG, const std::string &channel);
void	R332(REP_ARG, const std::string &channel, const std::string &topic);
//void	R333(REP_ARG, const std::string &channel, const std::string &nick, const std::string &setat);
void	R353(REP_ARG, const char &symbol, const std::string &channel, const std::string &members);
void	R366(REP_ARG, const std::string &channel);

/////////////////display TEST/////////////////////
static void    displayAllChannels(Server *server) //fonction de test modifiable a volonte
{
    for (size_t i = 0; i < server->getChannels().size(); i++)
    {
        std::cout << i << "channel: " << server->getChannels()[i]->getChannelName() << std::endl;
    }
}

void	join(Server *server, User *user, std::deque<std::string> tokens)
{
    if (tokens.size() <= 1 || tokens[1].empty()) //if not enough args
		E461(user->getFd(), user->getNick(), "JOIN");

	for (std::deque<std::string>::const_iterator it = tokens.begin() + 1; it != tokens.end(); it++)
	{
		if (!server->findChannel(*it))//if channel don't exist
			E403(user->getFd(), user->getNick(), *it);
		// else if you have joined too many channels
		//	E405(user->getFd(), user->getNick(), *it);
		//else if +k mode -> channel requires a key
		//	E475(user->getFd(), user->getNick(), *it);
		//else if +l mode -> maximum limit of user already joined channel
		//	E471(user->getFd(), user->getNick(), *it);
		//else if +i mode -> invite only and not invited to the channel
		//	E473(user->getFd(), user->getNick(), *it);

		//reussite
		//else
		//{
		//	if topic exist (send the current topic of the channel)
		//		R332(user->getFd(), user->getNick(), *it, topic);
		//commande NAMES <channel>      nicknames list of the joined members of the channel
		//	R353(user->getFd(), user->getNick(), status, channel->getName(), members); //status is '=' Public, '@' +s, '*' +p 
		//	R366(user->getFd(), user->getNick(), channel->getName()); //end of the list R353
		//}
	}

	if (tokens.size() <= 1 || tokens.size() > 3) //JOIN channel key uniquement, pas de multichannel
		return;
	for (size_t i = 1; i < tokens.size(); i++)
	{
		if (tokens.size() - i > 1)
			server->createChannel(user, tokens[i], tokens[i + 1]);
		server->createChannel(user, tokens[i]);
	}
	displayAllChannels(server);
    //send() ou send_to_client()dans Rep.cpp
}
