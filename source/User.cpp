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
	//this->_tokens = rhs.getTokens();
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

void	User::setPass( std::string str )
{
	//parse str for requirements
	this->_pass = str;
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
	//parse str for requirements
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

std::string	User::getPass( void ) const
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

////////////////////////functions////////////////////////

void	User::tokenizeMessage(std::string message)
{
	if (_tokens.size() > 0)
		_tokens.clear();
	size_t	j;
	for (size_t i = 0; i < message.length(); i++)
	{
		j = i;
		while (message[j] <= ' ' && message[j] != '\0')
			j++;
		i = j;
		while (message[j] > ' ' && message[j] != '\0')
			j++;
		if (j - i > 0)
			_tokens.push_back(message.substr(i, (j - i)));
		i = j;
	}
}

void	User::displayTokens( void ) const
{
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		std::cout << "displaying tokens: ";
		std::cout << _tokens[i] << std::endl;
	}
}