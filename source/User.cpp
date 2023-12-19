#include "User.hpp"

User::User( void ) : _logged_in(false)
{}

User::User( User const &src ) : _logged_in(src.getLoggedIn())
{
	*this = src;
}

User::~User() 
{}

User &User::operator=( User const &rhs ) 
{
	this->_fd = rhs.getFd();
	this->_logged_in = rhs.getLoggedIn();
	this->_pass = rhs.getPass();
	this->_nick = rhs.getNick();
	this->_username = rhs.getUsername();
	this->_message = rhs.getMessage();
	if (this->_tokens.size() > 0)
		this->_tokens.clear();
	for (size_t i = 0; i < rhs.getTokens().size(); i++)
		this->_tokens.push_back(rhs.getTokens()[i]);
	return *this;
}

////////////////////////setters////////////////////////
void	User::setFd( int fd )
{
	//parse str for requirements
	this->_fd = fd;
}

void	User::setLoggedIn( bool logged )
{
	this->_logged_in = logged;
}

void	User::setPass( bool pass_status )
{
	//parse str for requirements
	this->_pass = pass_status;
}

void	User::setNick( std::string str )
{
	//parse str for requirements
	this->_nick = str;
}

void	User::setUsername( std::string str )
{
	//parse str for requirements
	this->_username = str;
}

void	User::setMessage( std::string str )
{
	//gérer les ctrlD, donc gérer les messages jusqu'a recevoir un /n
	if (this->_message.empty())
		this->_message = str;
	else if (this->_message[this->_message.length() - 1] != '\n')
		this->_message.append(str);
	else
		this->_message = str;
}

////////////////////////getters////////////////////////

int	User::getFd( void ) const
{
	return (this->_fd);
}

bool	User::getLoggedIn( void ) const
{
	return (this->_logged_in);
}

bool	User::getPass( void ) const
{
	return (this->_pass);
}

std::string	User::getNick( void ) const
{
	return (this->_nick);
}

std::string	User::getUsername( void ) const
{
	return (this->_username);
}

std::string	User::getMessage( void ) const
{
	return (this->_message);
}

std::deque<std::string>	User::getTokens( void ) const
{
	return (this->_tokens);
}

////////////////////////functions////////////////////////

static int checkWhitespace(char c)
{
	if (c <= ' ' && c != '\n' && c != '\r')
		return 1;
	else
		return 0;
}

void	User::tokenizeMessage(std::string message)
{
	if (this->_tokens.size() > 0)
		this->_tokens.clear();
	size_t	j;
	for (size_t i = 0; i < message.find("\r\n"); i++)
	{
		j = i;
		while (checkWhitespace(message[j]) == 1 && message[j] != '\0')
			j++;
		i = j;
		if (message[i] == ':')
		{
			while (j < message.find("\r\n") && message[j] != '\0')
				j++;
		}
		else
		{
			while (checkWhitespace(message[j]) == 0 && j < message.find("\r\n") && message[j] != '\0')
				j++;
		}
		if (j - i > 0)
			this->_tokens.push_back(message.substr(i, (j - i)));
		i = j;
	}
}

void	User::displayTokens( void ) const //fonction test
{
	std::cout << "nb tokens: " << getTokens().size() << std::endl;
	for (size_t i = 0; i < this->_tokens.size(); i++)
	{
		std::cout << "token " << i << ": ";
		std::cout << this->_tokens[i] << std::endl;
	}
}