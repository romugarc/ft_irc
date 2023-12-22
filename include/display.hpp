#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "ft_irc.hpp"

class   Server;
class	User;
class	Channel;

void displayMessage(User *user);
void displayReply(User *user);
void displayAllUsers(Server *server);
void displayAllChannels(Server *server);
void	displayTokens(User *user);

#endif