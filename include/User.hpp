#ifndef USER_HPP
#define	USER_HPP

#include "ft_irc.hpp"

class	User
{
	private:
		//int	_fd;		
		// std::string _pass;
		// std::string _nick;
		// std::string	_username;
		// std::string	message;
		// std::deque<std::string> tokens;
	public:
		User( void );
		User( User const &src );
		~User();

		User &operator=( User const &rhs );

		//void	join(std::deque<std::string> tokens);

		////////////////parsing
		//void	parseMessage(std::string message);
		//void	tokenizeArguments(std::string message);
};

#endif