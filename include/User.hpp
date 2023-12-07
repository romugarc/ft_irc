#ifndef USER_HPP
#define	USER_HPP

#include "ft_irc.hpp"

class	User
{
	private:
		int	_fd;
		bool	_logged_in;
		std::string _pass;
		std::string _nick;
		std::string	_username;
		std::string	_message;
		std::deque<std::string> _tokens;
	public:
		User( void );
		User( User const &src );
		~User();

		User &operator=( User const &rhs );

		void	displayTokens( void ) const;

		void	setFd( int fd );
		void	setLoggedIn( bool logged );
		void	setPass( std::string str );
		void	setNick( std::string str );
		void	setUsername( std::string str );
		void	setMessage( std::string str );

		int	getFd( void ) const;
		bool	getLoggedIn( void ) const;
		std::string	getPass( void ) const;
		std::string	getNick( void ) const;
		std::string	getUsername( void ) const;
		std::string	getMessage( void ) const;
		char	getLastChar( void ) const;
		std::deque<std::string>	getTokens( void ) const;

		void execute( void );
		//void	join(std::deque<std::string> tokens);

		////////////////parsing
		void	tokenizeMessage(std::string message);
};

#endif