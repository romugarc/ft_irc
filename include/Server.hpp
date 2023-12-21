#ifndef SERVER_HPP
#define	SERVER_HPP

#include "Commands.hpp"

class User;
class Channel;

class Server
{
    private:
        std::string _host;
        std::string _port;
        std::string _password;
        int _socket_fd;
        std::deque<struct pollfd> _fds;
        std::deque<User *>      _users;
        std::deque<Channel *>   _channels;

    public:
        Server(void);
        Server(const Server &src);
        Server(std::string port, std::string password);
        ~Server(void);

        Server	&operator=(const Server &src);

        const std::string &getHost(void) const;
        const std::string &getPort(void) const;
        const std::string &getPassword(void) const;
        const std::deque<User *> &getUsers(void) const;
        const std::deque<Channel *> &getChannels(void) const;
    
        void    displayMessage(std::string message) const;
        void    displayAllUsers(void) const;
        void    displayAllChannels(void) const;

        void createSocket(void);
        void loop(void);
        void deleteSocket(void);

        void createUser(void);
        void deleteUser(int user_fd);
        void userMsg(int user_fd);
        void parseMsg(User *current_user);
        User *findUser(int fd);
        User *findUser(std::string nick);

        void createChannel(User *user_creator, std::string name);
        void deleteChannel( std::string channel_name );
        Channel *findChannel(std::string name);

		void execute( User *current_user );
};

#endif