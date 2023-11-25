#include "User.hpp"

User::User( void )
{}

User::User( User const &src ) 
{
	*this = src;
}

User::~User() 
{}

User &User::operator=( User const &rhs ) 
{
	(void)rhs;
	return *this;
}