#include "Server.hpp"

bool g_pascommun = 0;

Server::Server(void) { }

Server::Server(const Server &src) {*this = src;}

Server::Server(std::string port, std::string password) : _port(port), _password(password) { }

Server::~Server(void) 
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        delete _users[i];
    }
    for (size_t i = 0; i < _channels.size(); i++)
    {
        delete _channels[i];
    }
}

const std::string &Server::getHost(void) const
{
    return (_host);
}

const std::string &Server::getPort(void) const
{
    return (_port);
}

const std::string &Server::getPassword(void) const
{
    return (_password);
}

const std::deque<User *> &Server::getUsers(void) const
{
    return (_users);
}

const std::deque<Channel *> &Server::getChannels(void) const
{
    return (_channels);
}

Server  &Server::operator=(const Server &src)
{
    this->_port = src._port;
    this->_password = src._password;
	return (*this);
}

////////////////////////server////////////////////////

void    Server::createSocket(void)
{
    struct addrinfo sock_serv_param;
    struct addrinfo *servinfo;
    struct pollfd serv_fd;

    sock_serv_param.ai_family = AF_INET; // famille d'adresse IPV4/IPV6... (AF_INET = IPV4)
    sock_serv_param.ai_socktype = SOCK_STREAM; // type de socket TCP/UD/PIP... (SOCK_STREAM = TCP, for data integrity)
    sock_serv_param.ai_protocol = IPPROTO_TCP; // protocole (IPPROTO_TCP = TCP)
    sock_serv_param.ai_flags = AI_PASSIVE; // options supplementaires (AI_PASSIVE = autorise les connexions de n'importe quel hote)
    if (getaddrinfo(NULL, _port.c_str(), &sock_serv_param, &servinfo) < 0)
        throw std::runtime_error("Error: getaddrinfo()");

    _socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol); //cree la socket (IPV4, TCP, TCP)
    if (_socket_fd == -1)
    {
        freeaddrinfo(servinfo);
        throw std::runtime_error("Error: socket()");
    }
    int optvalue = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) == -1) // allow the re_use of a port id the IP address is different
    {
        freeaddrinfo(servinfo);
        throw std::runtime_error("Error: setsockopt()");
    }
    if (bind(_socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) //affecte l'adresse a la socket
    {
        freeaddrinfo(servinfo);
        throw   std::runtime_error("Error: bind()");
    }
    if (listen(_socket_fd, SOMAXCONN) == -1) //socket passive (autorise les connections entrantes avec accept())
    {
        freeaddrinfo(servinfo);
        throw   std::runtime_error("Error: listen()");
    }
    freeaddrinfo(servinfo);
    _host = "irc.42.serv";

    serv_fd.fd = _socket_fd;
    serv_fd.events = POLLIN; //POLLIN = attente de lecture | POLLOUT = ecriture non-bloquante
    _fds.push_back(serv_fd);
    std::cout << "Server Socket fd : " << _fds[0].fd << std::endl;
}

void    Server::deleteSocket(void)
{
    for (std::deque<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
    {
        if (close(i_pollfd->fd)) //close la socket de chaque user
            throw std::runtime_error("Error: close()");
        _fds.erase(i_pollfd); //erase le vecteur de chaque user
    }
    if (close(_socket_fd)) //close la socket du server
        throw std::runtime_error("Error: close()");
    std::cout << "Delete Server Socket fd : " << _socket_fd << std::endl;
}

////////////////////////user////////////////////////

void    Server::createUser(void)
{
    struct sockaddr_in sock_new_user;
    socklen_t sock_new_user_len = sizeof(sock_new_user);

    _fds.push_back(pollfd());
	_fds.back().fd = accept(_socket_fd, (struct sockaddr *)(&sock_new_user), &sock_new_user_len);
    _fds.back().events = POLLIN | POLLOUT;
    if (_fds.back().fd == -1)
        throw std::runtime_error("Error: accept()");
    std::cout << "User Connection Socket fd " << _fds.back().fd << std::endl;

    User    *newuser = new User;
    char hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)(&sock_new_user), sock_new_user_len, hostname, sizeof(hostname), NULL, 0, NI_NUMERICSERV))
		throw std::runtime_error("Error: getnameinfo()");
    newuser->setHostName(hostname);
    newuser->setFd(_fds.back().fd);
    _users.push_back(newuser);
}

void Server::deleteUser(int user_fd)
{
    for (std::deque<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
    {
        if (i_pollfd->fd == user_fd)
        {
            close(i_pollfd->fd);
            _fds.erase(i_pollfd);
            std::cout << "User Deconnection Socket fd : " << user_fd << std::endl;
            break;
        }
    }

    for (std::deque<Channel*>::iterator chan = _channels.begin(); chan < _channels.end();)
    {
        (*chan)->delUser(user_fd);
        (*chan)->delOperator(user_fd);
        if ((*chan)->getNbUser() < 1)
        {
            delete (*chan);
            chan = _channels.erase(chan);
        }
        else
            ++chan;
    }
    
    for (std::deque<User*>::iterator it = _users.begin(); it < _users.end(); it++)
    {
        if ((*it)->getFd() == user_fd)
        {
            delete (*it);
            _users.erase(it);
            break;
        }
    }
}

User *Server::findUser( int fd ) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (fd == _users[i]->getFd())
            return (_users[i]);
    }
    return (NULL);
}

User *Server::findUser(std::string nick) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (!nick.compare(_users[i]->getNick()))
            return (_users[i]);
    }
    return (NULL);
}

////////////////////////channel////////////////////////

void    Server::createChannel( User *user_creator, std::string name )
{
    Channel    *newchannel = new Channel;

    newchannel->addUser(user_creator);
    newchannel->addOperator(user_creator);
    newchannel->setName(name);
    _channels.push_back(newchannel);
}

void    Server::deleteChannel( std::string channel_name )
{
    for (std::deque<Channel*>::iterator it = _channels.begin(); it < _channels.end(); it++)
    {
        if ((*it)->getName() == channel_name)
        {
            delete (*it);
            _channels.erase(it);
            break;
        }
    }
}

Channel *Server::findChannel(std::string name) const
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (!name.compare(_channels[i]->getName()))
            return (_channels[i]);
    }
    return (NULL);
}

////////////////////////execution////////////////////////

void    Server::loop(void)
{
    //static int i;

    std::cout << GREEN << "GREEN Message Client" << RESET << std::endl;
    std::cout << CYAN << "CYAN Reply Server" << RESET << std::endl;

    _fds[0].revents = 0;
    while (g_pascommun == 0)
    {
        for (std::deque<struct pollfd>::iterator i_pollfd = _fds.begin(); i_pollfd < _fds.end(); i_pollfd++)
        {
            nfds_t n_fds = _fds.size();
            if (poll(&_fds[0], n_fds, -1) == -1 && !g_pascommun)
                throw std::runtime_error("Error server: poll");
            if (_fds[0].revents & POLLIN)
            {
                createUser();
            }
            else if (i_pollfd->revents & POLLIN)
            {
                userMsg(i_pollfd->fd);
                displayMessage(findUser(i_pollfd->fd));
                parseMsg(findUser(i_pollfd->fd));
            }
            if (i_pollfd->revents & POLLOUT)
            {
                User *user = findUser(i_pollfd->fd);
                if (user)
                {
                    if (user->getQuit() == true)
                        deleteUser(i_pollfd->fd);
                    else
                    {
                        if (!user->getReply().empty())
                        {
                            user->send_to_client();
                            //displayAllUsers(this);
                            //displayAllChannels(this);
                        }
                        user->setReply("");
                    }
                }
            }
        }
   }
}

void    Server::userMsg(int user_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t n = BUFFER_SIZE;
    std::string message;
    User    *user = findUser(user_fd);

    for (int j = 0; j < BUFFER_SIZE; j++)
    {
        buffer[j] = 0;
    }
    n = recv(user_fd, buffer, BUFFER_SIZE, 0);
    if (n == -1)
        throw std::runtime_error("Error: recv()");
    else if (n == 0)
    {
        if (close(user_fd))
            throw std::runtime_error("Error: close()");
        if (user != NULL)
            user->setQuit();
        return;
    }
    else
        message.insert(message.length(), buffer, static_cast<size_t>(n));

    if (user != NULL && user->getMessage().length() + static_cast<size_t>(n) < MSG_LIMIT)
        user->addMessage(message);
    else if (user != NULL)
    {
        user->addMessage(message);
        user->addMessage("\r\n");
    }
}

void    Server::parseMsg(User *current_user)
{
    if (current_user->getQuit() == true)
    {
        quit(this, current_user);
        return;
    }
    while (current_user->getMessage().find("\r\n") != std::string::npos)
    {
        current_user->tokenizeMessage(current_user->getMessage());
        current_user->setMessage(current_user->getMessage().substr(current_user->getMessage().find("\r\n") + 2, current_user->getMessage().length()));
        execute(current_user);
        if (current_user->getQuit() == true)
            return;
    }
}

void	Server::execute( User *current_user )
{
	std::string	commands[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "KICK", "INVITE", "TOPIC", "PRIVMSG", "PART", "WHOIS", "NOTICE", "QUIT"};
    int	i = 0;

    if (current_user->getTokens().size() <= 0)
    {
        return;
    }
	while (current_user->getTokens()[0] != commands[i] && i++ < 12);

	switch (i)
	{
		case 0:
			pass(this, current_user, current_user->getTokens());
			break;
		case 1:
			nick(this, current_user, current_user->getTokens());
			break;
		case 2:
			user(this, current_user, current_user->getTokens());
			break;
		case 3:
			join(this, current_user, current_user->getTokens());
			break;
		case 4:
			mode(this, current_user, current_user->getTokens());
			break;
        case 5:
            kick(this, current_user, current_user->getTokens());
            break;
        case 6:
            invite(this, current_user, current_user->getTokens());
            break;
        case 7:
            topic(this, current_user, current_user->getTokens());
            break;
        case 8:
            privmsg(this, current_user, current_user->getTokens());
            break;
        case 9:
            part(this, current_user, current_user->getTokens());
            break;
        case 10:
            whois(this, current_user, current_user->getTokens());
            break;
        case 11:
            notice(this, current_user, current_user->getTokens());
            break;
        case 12:
            quit(this, current_user, current_user->getTokens());
            break;
		default:
			E421(_host, current_user);
			break;
	}

    if (current_user->getLoggedIn() == false)
    {
        if (current_user->getPass() == true && !current_user->getUsername().empty() && !current_user->getNick().empty())
        {
            current_user->setLoggedIn(true);
            R001(this->_host, current_user);
        }
    }
}
