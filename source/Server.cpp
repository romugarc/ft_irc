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

void    Server::createSocket(void)
{
    struct addrinfo sock_serv_param;
    struct addrinfo *servinfo;

    sock_serv_param.ai_family = AF_INET; // famille d'adresse IPV4/IPV6... (AF_INET = IPV4)
    sock_serv_param.ai_socktype = SOCK_STREAM; // type de socket TCP/UD/PIP... (SOCK_STREAM = TCP, for data integrity)
    sock_serv_param.ai_protocol = IPPROTO_TCP; // protocole (IPPROTO_TCP = TCP)
    sock_serv_param.ai_flags = AI_PASSIVE; // options supplementaires (AI_PASSIVE = autorise les connexions de n'importe quel hote)
    if (getaddrinfo(NULL, _port.c_str(), &sock_serv_param, &servinfo) < 0)
        throw std::runtime_error("Error: getaddrinfo()");

    /*std::cout << "port >> " << _port.c_str() << std::endl;
    std::cout << "addrinfo >> flags: " << sock_serv_param.ai_flags << " | family: " << sock_serv_param.ai_family << " | socktype: " << sock_serv_param.ai_socktype << " | protocol: " << sock_serv_param.ai_protocol <<
    " | addrlen: " << sock_serv_param.ai_addrlen << " | addr: " << sock_serv_param.ai_addr << " | next: " << sock_serv_param.ai_next << std::endl;
    std::cout << "servinfo >> flags: " << servinfo->ai_flags << " | family: " << servinfo->ai_family << " | socktype: " << servinfo->ai_socktype << " | protocol: " << servinfo->ai_protocol <<
    " | addrlen: " << servinfo->ai_addrlen << " | addr: " << servinfo->ai_addr << " | next: " << servinfo->ai_next << std::endl;*/

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
    char hostname[NI_MAXHOST];
	if (getnameinfo(servinfo->ai_addr, servinfo->ai_addrlen, hostname, sizeof(hostname), NULL, 0, NI_NUMERICHOST))
    {
        freeaddrinfo(servinfo);
		throw std::runtime_error("Error: getnameinfo()");
    }
    freeaddrinfo(servinfo);
    _host = hostname;
    std::cout << "Server Socket fd : " << _socket_fd << std::endl;
}

void    Server::loop(void)
{
    struct pollfd serv_fd; //struct pollfd => .events = parametres d'entree, .revents = parametres de sortie detecte
    static int i;
    serv_fd.fd = _socket_fd;
    serv_fd.events = POLLIN; //POLLIN = attente de lecture | POLLOUT = ecriture non-bloquante
    _fds.push_back(serv_fd); //deque<struct pollfd>

    std::cout << GREEN << "GREEN Message Client" << RESET << std::endl;
    std::cout << CYAN << "CYAN Reply Server" << RESET << std::endl;

    std::cout << g_pascommun << std::endl;
    _fds[0].revents = 0;
    while (g_pascommun == 0)
    {
        i++;
        std::cout << "\r(Poll " << i << ") " << std::flush;

        nfds_t n_fds = _fds.size(); //nfds_t = size_t pour pollfd
        if (poll(&_fds[0], n_fds, -1) == -1 && !g_pascommun)
            throw std::runtime_error("Error server: poll");
        if (_fds[0].revents & POLLIN) //si un event POLLIN sur le serv
        {
            createUser();
        }
        for (std::deque<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
        {
            if (i_pollfd->revents & POLLIN) //si un event POLLIN sur un user
            {
                userMsg(i_pollfd->fd);
                if (findUser(i_pollfd->fd)->getQuit() == true)
                    deleteUser(i_pollfd->fd);
                displayAllUsers();
                displayAllChannels();
            }
            if (i_pollfd->revents & POLLOUT) //si un event POLLOUT sur un user
            {
                //servMsgtoUser(i_pollfd->fd);
            }
        }
   }
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

void    Server::createUser(void)
{
    struct sockaddr_in sock_new_user; //sockaddr_in = ipv4 internet domain
    socklen_t sock_new_user_len = sizeof(sock_new_user);

    _fds.push_back(pollfd());
	_fds.back().fd = accept(_socket_fd, (struct sockaddr *)(&sock_new_user), &sock_new_user_len); //accepte la connexion user (POLLIN) et retourne la socket user
    _fds.back().events = POLLIN | POLLOUT;
    if (_fds.back().fd == -1)
        throw std::runtime_error("Error: accept()");
    // if (fcntl(_fds.back().fd, F_SETFL, O_NONBLOCK) == -1)
    //     throw std::runtime_error("Error: fcntl()");
    /*int flags = fcntl(_fds.back().fd, F_GETFL, 0);
    if (flags & O_NONBLOCK)
        std::cout << "non_bloquant" << std::endl;
    else
        std::cout << "bloquant" << std::endl;*/
    std::cout << "User Connection fd " << _fds.back().fd << std::endl;

    User    *newuser = new User;
    char hostname[NI_MAXHOST];

	if (getnameinfo((struct sockaddr *)(&sock_new_user), sock_new_user_len, hostname, sizeof(hostname), NULL, 0, NI_NUMERICHOST))
		throw std::runtime_error("Error: getnameinfo()");
    newuser->setHostName(hostname);
    newuser->setFd(_fds.back().fd);
    _users.push_back(newuser);
}

void Server::deleteUser(int user_fd)
{
    //enlever le user de la liste
    for (std::deque<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
    {
        if (i_pollfd->fd == user_fd)
        {
            close(i_pollfd->fd);
            _fds.erase(i_pollfd);
            std::cout << "User Deconnection fd : " << user_fd << std::endl;
            break;
        }
    }

    for (std::deque<Channel*>::iterator chan = _channels.begin(); chan < _channels.end(); chan++)
    {
        (*chan)->delUser(user_fd);
        (*chan)->delOperator(user_fd);
        if ((*chan)->getNbUser() < 1)
        {
            delete (*chan);
            _channels.erase(chan);
        } //tester si l'iterateur est coherent en creant plusieurs channels avec ce user en dernier puis en supprimant ce user
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

void    Server::userMsg(int user_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t n = BUFFER_SIZE;
    std::string message;

    for (int j = 0; j < BUFFER_SIZE; j++)
        buffer[j] = 0;
    while (n == BUFFER_SIZE)
    {
        n = recv(user_fd, buffer, BUFFER_SIZE, 0);
        if (n == -1)
            throw std::runtime_error("Error: recv()");
        else if (n == 0)
        {
            if (close(user_fd))
                throw std::runtime_error("Error: close()");
            quit(this, findUser(user_fd));
            return;
        }
        else
        {
            message.insert(message.length(), buffer, static_cast<size_t>(n));
        }
    }
    std::cout << "User fd " << user_fd << " : " << std::endl;
    displayMessage(message);

    User    *user = findUser(user_fd);
    if (user != NULL)
        parseMsg(user, message);
}

void    Server::parseMsg(User *current_user, std::string message)
{
    current_user->setMessage(message);

    while (message.find("\r\n") != std::string::npos)
    {
        current_user->tokenizeMessage(message);
        message = message.substr(message.find("\r\n") + 2, message.length());
        execute(current_user);
        if (current_user->getQuit() == true)
            return;
    }

    if (current_user->getLoggedIn() == false)
    {
        if (current_user->getPass() == true && !current_user->getUsername().empty() && !current_user->getNick().empty())
        {
            current_user->setLoggedIn(true);
            R001(current_user->getFd(), this->_host, current_user->getNick(), current_user->getUsername(), current_user->getHostName());
        }
    }
}

User *Server::findUser( int fd )
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (fd == _users[i]->getFd())
            return (_users[i]);
    }
    return (NULL); //jamais null dans userMsg
}

User *Server::findUser(std::string nick)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (!nick.compare(_users[i]->getNick()))
            return (_users[i]);
    }
    return (NULL);
}

void    Server::createChannel( User *user_creator, std::string name, std::string key )
{
    Channel    *newchannel = new Channel;

    newchannel->addUser(user_creator);
    newchannel->addOperator(user_creator);
    newchannel->setName(name);
    newchannel->setKey(key);
    _channels.push_back(newchannel);
}

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

Channel *Server::findChannel(std::string name)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (!name.compare(_channels[i]->getName()))
            return (_channels[i]);
    }
    return (NULL);
}

////////////////////////execution////////////////////////

void	Server::execute( User *current_user )
{
	std::string	commands[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "KICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT"};
    int	i = 0;

    if (current_user->getTokens().size() <= 0)
    {
        return;
    }
	while (current_user->getTokens()[0] != commands[i] && i++ < 9);

	switch (i) //agrandir ce switch au fur et a mesure
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
            quit(this, current_user, current_user->getTokens());
            break;
		default:
			//throw exception?
			break;
	}
}

////////////////////////displays for testing////////////////////////

void Server::displayMessage(std::string message) const
{
    std::cout << GREEN;
    for(std::string::iterator it=message.begin(); it!=message.end(); it++)
    {
        if (*it == '\r')
            std::cout << "\\r";
        else if (*it == '\n')
            std::cout << "\\n";
        else
            std::cout << *it;
    }
    std::cout << RESET << std::endl;
}

void    Server::displayAllUsers(void) const //fonction de test modifiable a volonte
{
    std::cout << "###################################################USERS###################################################" << std::endl;
    std::cout << "FD\t\tHostname\tNickname\tUsername\tPWD Status\tStatus\t\tLast Message" << std::endl;
    for (size_t i = 0; i < _users.size(); i++)
    {
        std::cout << _users[i]->getFd() << "\t\t";
        std::cout << _users[i]->getHostName() << "\t";
        if (_users[i]->getNick().length() > 12)
            std::cout << _users[i]->getNick().substr(0, 12) << "...\t";
        else if (_users[i]->getNick().length() > 7)
            std::cout << _users[i]->getNick() << "\t";
        else
            std::cout << _users[i]->getNick() << "\t\t";
        if (_users[i]->getUsername().length() > 12)
            std::cout << _users[i]->getUsername().substr(0 ,12) << "...\t";
        else if (_users[i]->getUsername().length() > 7)
            std::cout << _users[i]->getUsername() << "\t";
        else
            std::cout << _users[i]->getUsername() << "\t\t";
        std::cout << _users[i]->getPass() << "\t\t";
        std::cout << _users[i]->getLoggedIn() << "\t\t";
        displayMessage(_users[i]->getMessage());
        std::cout << std::endl;
    }
}

void    Server::displayAllChannels(void) const
{
    std::cout << "###################################################CHANNELS###################################################" << std::endl;
	std::cout << "Name\t\tKey\t\tTopic\t\tModes\t\tnb_user_limit\tnb_user\t\tUserlist" << std::endl;
    for (size_t i = 0; i < _channels.size(); i++)
    {
        std::cout << _channels[i]->getName() << "\t\t";
		std::cout << _channels[i]->getKey() << "\t\t";
		std::cout << _channels[i]->getTopic() << "\t\t";
		std::cout << _channels[i]->getModes() << "\t\t";
		std::cout << _channels[i]->getNbUserLimit() << "\t\t";
		std::cout << _channels[i]->getNbUser() << "\t\t";
		for (size_t j = 0; j < _channels[i]->getUserList().size(); j++)
			std::cout << _channels[i]->getUserList()[j]->getNick() << ", ";
		std::cout << std::endl;
    }
}