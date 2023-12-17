#include "ft_irc.hpp"

int	join(Server *server, User *current_user, std::deque<std::string> tokens)
{
	if (tokens.size() <= 1 || tokens.size() > 3) //JOIN channel key uniquement, pas de multichannel
		return 0;
	for (size_t i = 1; i < tokens.size(); i++)
	{
		if (tokens.size() - i > 1)
			server->createChannel(current_user, tokens[i], tokens[i + 1]);
		server->createChannel(current_user, tokens[i]);
	}
    std::cout << CYAN << "exemple texte JOIN" << RESET << std::endl;
    //send() ou send_to_client()dans Rep.cpp
	return 1;
}