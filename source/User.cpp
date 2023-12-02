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

std::deque<std::string>	User::getTokens( void ) const
{
	return (this->_tokens);
}

////////////////////////functions////////////////////////

void	User::tokenizeMessage(std::string message)
{
	if (this->_tokens.size() > 0)
		this->_tokens.clear();
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
			this->_tokens.push_back(message.substr(i, (j - i)));
		i = j;
	}
}

void	User::displayTokens( void ) const //fonction test
{
	for (size_t i = 0; i < this->_tokens.size(); i++)
	{
		std::cout << "displaying tokens: ";
		std::cout << this->_tokens[i] << std::endl;
	}
}

void	User::execute( void ) //prototype a changer surement
{
	std::string	commands[1] = {"JOIN"}; //, "KICK", "INVITE", "TOPIC", "MODE"}; //ajouter fonctions au jur et a mesure
	int	i = 0;

	if (this->_tokens.size() > 0)
		while (this->_tokens[0] != commands[i] && i++ < 5);

	switch (i) //agrandir ce switch au fur et a mesure
	{
		case 0:
			//join
			break;
		default:
			//throw exception?
			break;
	}
}