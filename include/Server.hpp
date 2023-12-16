#ifndef SERVER_HPP
#define	SERVER_HPP

#include "ft_irc.hpp"

class User;
class Channel;

class Server
{
    private:
        std::string _port;
        std::string _password;
        int _socket_fd;
        std::vector<struct pollfd> _fds; //transformer en deque
        std::deque<User *>      _users;
        std::deque<Channel *>   _channels;

    public:
        Server(void);
        Server(const Server &src);
        Server(std::string port, std::string password);
        ~Server(void);

        Server	&operator=(const Server &src);

        const std::string &getPort(void) const;
        const std::string &getPassword(void) const;
        void    displayAllUsers(void) const;
        void    displayAllChannels(void) const;

        void createSocket(void);
        void loop(void);
        void deleteSocket(void);

        void createUser(void);
        void deleteUser(int user_fd);
        void userMsg(int user_fd);

        void createChannel(User *user_creator, std::string name, std::string key);
        void createChannel(User *user_creator, std::string name);

		void execute( User *current_user );
		int	join( User *current_user, std::deque<std::string> tokens );

        User    *findUser(int fd);
};

#endif