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
		std::deque<User*> _userlist; //delete quand un user est delete dans le server
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

		std::string	getChannelName( void ) const;
		std::string	getChannelKey( void ) const;
		std::deque<User*> getUserList( void ) const;
		std::deque<User*> getOpList( void ) const;
		std::deque<User*> getBanList( void ) const;


		std::deque<User*>	getOpList( void );
};

#endif