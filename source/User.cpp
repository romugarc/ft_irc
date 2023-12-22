#include "User.hpp"

User::User( void ) : _logged_in(false), _pass(false), _quit(false), _nb_chan_limit(0)
{
	_modes = "";
	_nick = "";
	_username = "";
	_realname = "";
	_message = "";
	_reply = "";
}

User::User( User const &src ) : _logged_in(src.getLoggedIn()), _nb_chan_limit(src.getNbChanLimit())
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
	this->_realname = rhs.getRealname();
	this->_message = rhs.getMessage();
	this->_reply = rhs.getReply();
	if (this->_tokens.size() > 0)
		this->_tokens.clear();
	for (size_t i = 0; i < rhs.getTokens().size(); i++)
		this->_tokens.push_back(rhs.getTokens()[i]);
	return *this;
}

////////////////////////setters////////////////////////

void	User::setHostName( std::string hostname )
{
	this->_hostname = hostname;
}

void	User::setFd( int fd )
{
	this->_fd = fd;
}

void	User::setLoggedIn( bool logged )
{
	this->_logged_in = logged;
}

void	User::setPass( bool pass_status )
{
	this->_pass = pass_status;
}

void	User::setNick( std::string str )
{
	this->_nick = str;
}

void	User::setUsername( std::string str )
{
	this->_username = str;
}

void	User::setRealname( std::string str )
{
	this->_realname = str;
}

void	User::setMessage( std::string str )
{
	this->_message = str;
}

void	User::addMessage( std::string str )
{
	this->_message.append(str);
}

void	User::setReply( std::string str )
{
	this->_reply = str;
}

void	User::addReply( std::string str )
{
	this->_reply.append(str);
}

void	User::setNbChanLimit( int limit )
{
	this->_nb_chan_limit = limit;
}

void	User::setNbChan( int nb_chan )
{
	this->_nb_chan = nb_chan;
}

void	User::setQuit( void )
{
	this->_quit = true;
}

////////////////////////getters////////////////////////

std::string	User::getHostName( void ) const
{
	return (this->_hostname);
}

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

bool	User::getQuit( void ) const
{
	return (this->_quit);
}

std::string	User::getNick( void ) const
{
	return (this->_nick);
}

std::string	User::getUsername( void ) const
{
	return (this->_username);
}

std::string	User::getRealname( void ) const
{
	return (this->_realname);
}

std::string	User::getMessage( void ) const
{
	return (this->_message);
}

std::string	User::getReply( void ) const
{
	return (this->_reply);
}

std::deque<std::string>	User::getTokens( void ) const
{
	return (this->_tokens);
}

int	User::getNbChanLimit( void ) const
{
	return (this->_nb_chan_limit);
}

int	User::getNbChan( void ) const
{
	return (this->_nb_chan);
}

std::string	User::getModes( void ) const
{
	return (this->_modes);
}

std::string User::getClientId( void ) const
{
    std::string id;

    id = _nick + "!" + _username + "@" + _hostname;
    return (id);
}

////////////////////////functions////////////////////////

void	User::send_to_client(void)
{
	displayReply(this);
	if (send(_fd, _reply.c_str(), _reply.size(), 0) == -1)
		throw std::runtime_error("Error: send()");
}

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
			i++;
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
	//displayTokens(this);
}
