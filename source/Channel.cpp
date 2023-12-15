#include "Channel.hpp"

Channel::Channel( void )
{}

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

////////////////////////setters////////////////////////

void	Channel::setOperator( User *new_op )
{
	int	is_present = 0;

	for (size_t i = 0; i < this->_oplist.size(); i++)
	{
		if (new_op->getFd() == this->_oplist[i]->getFd())
		{
			is_present = 1;
			break;
		}
	}
	if (is_present == 0)
		this->_oplist.push_back(new_op);
	//voir si il faut afficher un message dans le cas où le user est deja op
}

void	Channel::setChannelName( std::string name )
{
	this->_chan_name = name;
}

void	Channel::setChannelKey( std::string key )
{
	this->_chan_key = key;
}

////////////////////////getters////////////////////////

std::deque<User *>	Channel::getOpList( void )
{
	return (this->_oplist);
}

std::string	Channel::getChannelName( void )
{
	return (this->_chan_name);
}

std::string	Channel::getChannelKey( void )
{
	return (this->_chan_key);
}