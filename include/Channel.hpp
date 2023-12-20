#ifndef CHANNEL_HPP
#define	CHANNEL_HPP

#include "ft_irc.hpp"

class User;

class	Channel
{
	private:
		std::string	_name;
		std::string	_key;
		std::string	_topic;
		std::string	_modes;
		std::deque<User*> _userlist; //delete quand un user est delete dans le server
		std::deque<User*> _oplist;
		std::deque<User*> _banlist;
		int	_nb_user_limit;
		int	_nb_user;

	public:
		Channel( void );
		Channel( Channel const &src );
		~Channel();

		Channel &operator=( Channel const &rhs );

		void	setName( std::string name );
		void	setKey( std::string key );
		void	setTopic( std::string topic );
		//void	addMode( char mode );
		//void	removeMode( char mode );
		void	setNbUserLimit( int limit );
		void	setNbUser( int nb_user );
		void	addUser( User *new_user );
		void	delUser( int user_fd );
		User	*findUser( int fd );
		User	*findUser( std::string nick );
		void	addOperator( User *new_op );
		void	delOperator( int user_fd );
		User	*findOperator( int fd );


		std::string	getName( void ) const;
		std::string	getKey( void ) const;
		std::string	getTopic( void ) const;
		std::string	getModes( void ) const;
		int	getNbUserLimit( void ) const;
		int	getNbUser( void ) const;
		std::deque<User*> getUserList( void ) const;
		std::deque<User*> getOpList( void ) const;
		std::deque<User*> getBanList( void ) const;
};

#endif