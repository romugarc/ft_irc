#include "Rep.hpp"

//REP_ARG == int const &fd, const std::string &host, const std::string &client

static void displayReply(std::string message)
{
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

void	send_to_client(std::string msg, int const &fd)
{
	std::string	message;

	message = msg +"\r\n";
	displayReply(message);
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		throw std::runtime_error("Error: send()");
}

void	RJOIN(REP_ARG, const std::string &channel)
{
	(void)host;
	std::stringstream	output;

	output.str("");
	output << ":" << client << " JOIN " << channel;
	send_to_client(output.str(), fd);
}

void	RMODE(REP_ARG, const std::string &target, const char operation, const char mode)
{
	(void)host;
	std::stringstream	output;

	output.str("");
	output << ":" << client << " MODE " << target << " " << operation << mode;
	send_to_client(output.str(), fd);
}

void 	R001(REP_ARG, const std::string &user, const std::string &userhost)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 001 " << client << " :Welcome to the IRC 42 Network, " << client << "!" << user << "@" << userhost;
	send_to_client(output.str(), fd);
}

void	R221(REP_ARG, const std::string &user_modes)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 221 " << client << " +" << user_modes;
	send_to_client(output.str(), fd);
}


void	R324(REP_ARG, const std::string &channel, const std::string &mode)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 324 " << client << " " << channel << " +" << mode;
	send_to_client(output.str(), fd);
}

void	R331(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 331 " << client << " " << channel << " :No topic is set";
	send_to_client(output.str(), fd);
}

void	R332(REP_ARG, const std::string &channel, const std::string &topic)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 332 " << client << " " << channel << " :" << topic;
	send_to_client(output.str(), fd);
}

void	R341(REP_ARG, const std::string &channel, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 341 " << client << " " << channel << " " << input_name;
	send_to_client(output.str(), fd);
}

// void	R352(REP_ARG, const std::string &channel, const std::string &username, const std::string &nick)
// {
// 	std::stringstream	output;

// 	output.str("");
// 	output << ":" << host << " 352 " << client << " " << channel << " :" << username << " " << nick;
// 	send_to_client(output.str(), fd);
// }

void	R353(REP_ARG, const char &symbol, const std::string &channel, const std::string &prefix, const std::string &nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 353 " << client << " " << symbol << " " << channel << " :" << prefix << nick;
	send_to_client(output.str(), fd);
}

void	R366(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 366 " << client << " " << channel << " :End of /NAMES list";
	send_to_client(output.str(), fd);
}

void	R372(REP_ARG, const std::string &line)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 372 " << client << " : " << line;
	send_to_client(output.str(), fd);
}

void	R375(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 375 " << client << " :- ircserv Message of the day - ";
	send_to_client(output.str(), fd);
}

void	R376(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 376 " << client << " :End of MOTD command";
	send_to_client(output.str(), fd);
}

void	R391(REP_ARG)
{
	std::stringstream	output;
	char 				date_string[50];
	time_t 				curr_time;
	tm 					*curr_tm;

	output.str("");
	time(&curr_time);
	curr_tm = localtime(&curr_time);
	strftime(date_string, 50, "%c", curr_tm);
	output << ":" << host << " 391 " << client << " ircserv :" << date_string;
	send_to_client(output.str(), fd);
}

void	E401(REP_ARG, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 401 " << client << " " << input_name << " :No such nick/channel";
	send_to_client(output.str(), fd);
}

void	E402(REP_ARG, const std::string &server)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 402 " << client << " " << server << " :No such server";
	send_to_client(output.str(), fd);
}

void 	E403(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 403 " << client << " " << channel << " :No such channel";
	send_to_client(output.str(), fd);
}

void 	E404(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 404 " << client << " " << channel << " :Cannot send to channel";
	send_to_client(output.str(), fd);
}

void	E405(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 405 " << client << " " << channel <<  " :You have joined too many channels";	
	send_to_client(output.str(), fd);
}

void	E411(REP_ARG, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 411 " << client << " :No recipient given (" << cmd << ")";
	send_to_client(output.str(), fd);
}

void	E412(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 412 " << client << " :No text to send";
	send_to_client(output.str(), fd);
}

void	E421(REP_ARG, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 421 " << client << " " << cmd << " :Unknown command";
	send_to_client(output.str(), fd);
}

void	E422(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 422 " << client << " :MOTD File is missing";
	send_to_client(output.str(), fd);
}

void	E431(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 431 " << client << " :No nickname given";
	send_to_client(output.str(), fd);
}

void	E432(REP_ARG, const std::string &nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 432 " << client << " " << nick << " :Erroneous nickname";
	send_to_client(output.str(), fd);
}

void	E433(REP_ARG, const std::string &nick)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 433 " << client << " " << nick << " :Nickname is already in use";
	send_to_client(output.str(), fd);
}

void	E441(REP_ARG, const std::string &channel, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 441 " << client << " " << input_name << " " << channel << " :They aren't on that channel";
	send_to_client(output.str(), fd);
}

void	E442(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 442 " << client << " " << channel << " :You're not on that channel";
	send_to_client(output.str(), fd);
}

void	E443(REP_ARG, const std::string &channel, const std::string &input_name)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 443 " << client << " " << input_name << " " << channel << " :is already on channel";
	send_to_client(output.str(), fd);
}

void	E461(REP_ARG, const std::string &cmd)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 461 " << client << " " << cmd << " :Not enough parameters";
	send_to_client(output.str(), fd);
}

void	E462(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 462 " << client << " :Unauthorized command (already registered)";
	send_to_client(output.str(), fd);
}

void	E464(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 464 " << client << " :Password incorrect";
	send_to_client(output.str(), fd);
}

void	E471(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 471 " << client << " " << channel << " :Cannot join channel (+l)";
	send_to_client(output.str(), fd);
}

void 	E472(REP_ARG, const std::string &channel, const char &mode)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 472 " << client << " " << mode << " :is unknown mode char to me for "
	<< channel;
	send_to_client(output.str(), fd);
}

void	E473(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 473 " << client << " " << channel << " :Cannot join channel (+i)";
	send_to_client(output.str(), fd);
}

void	E475(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 475 " << client << " " << channel << " :Cannot join channel (+k)";
	send_to_client(output.str(), fd);
}

void	E482(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 482 " << client << " " << channel << " :You're not channel operator";
	send_to_client(output.str(), fd);
}

void	E501(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 501 " << client << " :Unknown MODE flag";
	send_to_client(output.str(), fd);
}

void	E502(REP_ARG)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 502 " << client << " :Cant change mode for other users";
	send_to_client(output.str(), fd);
}

void	E525(REP_ARG, const std::string &channel)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 525 " << client << " " << channel << " :Key is not well-formed";
	send_to_client(output.str(), fd);
}

void	E696(REP_ARG, const std::string &channel, const std::string &mode, const std::string &password)
{
	std::stringstream	output;

	output.str("");
	output << ":" << host << " 696 " << client << " " << channel << " " << mode << " " << password << " :limit must only contained digit";
	send_to_client(output.str(), fd);
}
