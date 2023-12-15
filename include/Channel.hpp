#ifndef CHANNEL_HPP
#define	CHANNEL_HPP

#include "ft_irc.hpp"

class User;

class	Channel
{
	private:
		//std::string _pass;
		std::string	_chan_name;
		std::string	_chan_key;
		std::deque<User*> _userlist;
		std::deque<User*> _oplist;
		std::deque<User*> _banlist;
	public:
		Channel( void );
		Channel( Channel const &src );
		~Channel();

		Channel &operator=( Channel const &rhs );

		void	setOperator( User *new_op );
		void	setChannelName( std::string name );
		void	setChannelKey( std::string key );

		std::string	getChannelName( void );
		std::string	getChannelKey( void );

		std::deque<User*>	getOpList( void );
};

#endif