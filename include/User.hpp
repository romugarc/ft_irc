#ifndef USER_HPP
#define	USER_HPP

#include "ft_irc.hpp"

class Server;

class	User
{
	private:
		std::string	_hostname;
		int		_fd;
		bool	_logged_in;
		bool	_pass;
		bool	_quit;
		std::string _nick;
		std::string	_username;
		std::string	_realname;
		std::string	_message;
		std::deque<std::string> _tokens;
		std::string	_reply;
		int	_nb_chan_limit;
		int	_nb_chan;
		std::string	_modes;

	public:
		User( void );
		User( User const &src );
		~User();

		User &operator=( User const &rhs );

		void	setHostName( std::string hostname );
		void	setFd( int fd );
		void	setLoggedIn( bool logged );
		void	setPass( bool pass_status );
		void	setQuit( void );
		void	setNick( std::string str );
		void	setUsername( std::string str );
		void	setRealname( std::string str );
		void	setMessage( std::string str );
		void	setReply( std::string str );
		void	addReply( std::string str );
		void	setNbChanLimit( int limit );
		void	setNbChan( int nb_chan );

		std::string	getHostName( void ) const;
		int	getFd( void ) const;
		bool	getLoggedIn( void ) const;
		bool	getPass( void ) const;
		bool	getQuit( void ) const;
		std::string	getNick( void ) const;
		std::string	getUsername( void ) const;
		std::string	getRealname( void ) const;
		std::string	getMessage( void ) const;
		std::string	getReply( void ) const;
		std::deque<std::string>	getTokens( void ) const;
		int	getNbChanLimit( void ) const;
		int	getNbChan( void ) const;
		std::string	getModes( void ) const;
		std::string getClientId( void ) const;
		
		void	send_to_client();
		void	tokenizeMessage(std::string message);
};

#endif