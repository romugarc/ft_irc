#include "Channel.hpp"

Channel::Channel( void ) : _nb_user_limit(0), _nb_user(0)
{
	_modes = "";
}

Channel::Channel( Channel const &src )
{
	*this = src;
}

Channel::~Channel()
{}

Channel &Channel::operator=( Channel const &rhs )
{
	(void)rhs;
	return *this;
}

////////////////////////channel operations////////////////////////

void	Channel::addUser( User *new_user )
{
	int	is_present = 0;

	for (size_t i = 0; i < _userlist.size(); i++)
	{
		if (new_user->getFd() == _userlist[i]->getFd())
		{
			is_present = 1;
			break;
		}
	}
	if (is_present == 0)
	{
		_nb_user++;
		_userlist.push_back(new_user);
	}
}

void	Channel::delUser( int user_fd )
{
    for (std::deque<User*>::iterator it = _userlist.begin(); it < _userlist.end(); it++)
    {
        if ((*it)->getFd() == user_fd)
        {
            _userlist.erase(it);
			_nb_user--;
            break;
        }
    }
}

User *Channel::findUser( int fd )
{
    for (size_t i = 0; i < _userlist.size(); i++)
    {
        if (fd == _userlist[i]->getFd())
            return (_userlist[i]);
    }
    return (NULL);
}

User *Channel::findUser(std::string nick)
{
    for (size_t i = 0; i < _userlist.size(); i++)
    {
        if (!nick.compare(_userlist[i]->getNick()))
            return (_userlist[i]);
    }
    return (NULL);
}

void	Channel::addOperator( User *new_op )
{
	int	is_present = 0;

	for (size_t i = 0; i < _oplist.size(); i++)
	{
		if (new_op->getFd() == _oplist[i]->getFd())
		{
			is_present = 1;
			break;
		}
	}
	if (is_present == 0)
		_oplist.push_back(new_op);
}

void	Channel::delOperator( int user_fd )
{
    for (std::deque<User*>::iterator it = _oplist.begin(); it < _oplist.end(); it++)
    {
        if ((*it)->getFd() == user_fd)
        {
            _oplist.erase(it);
            break;
        }
    }
}

User *Channel::findOperator( int fd )
{
    for (size_t i = 0; i < _oplist.size(); i++)
    {
        if (fd == _oplist[i]->getFd())
            return (_oplist[i]);
    }
    return (NULL);
}

void	Channel::addInvited( User *new_inv )
{
	int	is_present = 0;

	for (size_t i = 0; i < _invitedlist.size(); i++)
	{
		if (new_inv->getFd() == _invitedlist[i]->getFd())
		{
			is_present = 1;
			break;
		}
	}
	if (is_present == 0)
		_invitedlist.push_back(new_inv);
}

void	Channel::delInvited( int user_fd )
{
    for (std::deque<User*>::iterator it = _invitedlist.begin(); it < _invitedlist.end(); it++)
    {
        if ((*it)->getFd() == user_fd)
        {
            _invitedlist.erase(it);
            break;
        }
    }
}

User *Channel::findInvited( int fd )
{
    for (size_t i = 0; i < _invitedlist.size(); i++)
    {
        if (fd == _invitedlist[i]->getFd())
            return (_invitedlist[i]);
    }
    return (NULL);
}

////////////////////////setters////////////////////////

void	Channel::setName( std::string name )
{
	this->_name = name;
}

void	Channel::setKey( std::string key )
{
	this->_key = key;
}

void	Channel::setTopic( std::string topic )
{
	this->_topic = topic;
}

void	Channel::addMode( char mode )
{
	if (this->_modes.find(mode) != std::string::npos)
		return;
	this->_modes += mode;
}

void	Channel::removeMode( char mode )
{
	while (this->_modes.find(mode) != std::string::npos)
		this->_modes.erase(this->_modes.find(mode), 1);
}

void	Channel::setNbUserLimit( int limit )
{
	this->_nb_user_limit = limit;
}

void	Channel::setNbUser( int nb_user )
{
	this->_nb_user = nb_user;
}

////////////////////////getters////////////////////////

std::string	Channel::getName( void ) const
{
	return (this->_name);
}

std::string	Channel::getKey( void ) const
{
	return (this->_key);
}

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}

std::string	Channel::getModes( void ) const
{
	return (this->_modes);
}

int	Channel::getNbUserLimit( void ) const
{
	return (this->_nb_user_limit);
}

int	Channel::getNbUser( void ) const
{
	return (this->_nb_user);
}

std::deque<User *>	Channel::getUserList( void ) const
{
	return (this->_userlist);
}

std::deque<User *>	Channel::getOpList( void ) const
{
	return (this->_oplist);
}

std::deque<User *>	Channel::getInvList( void ) const
{
	return (this->_invitedlist);
}