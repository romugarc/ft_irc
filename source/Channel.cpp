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