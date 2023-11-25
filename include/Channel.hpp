#ifndef CHANNEL_HPP
#define	CHANNEL_HPP

#include "ft_irc.hpp"

class	Channel
{
	private:
		//int	_fd;
		//std::string _pass;
		//std::deque<User*> _userlist;
		//std::deque<User*> _oplist;
	public:
		Channel( void );
		Channel( Channel const &src );
		~Channel();

		Channel &operator=( Channel const &rhs );
};

#endif