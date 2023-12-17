#include "Server.hpp"

Server::Server(void) { }

Server::Server(const Server &src) {*this = src;}

Server::Server(std::string port, std::string password) : _port(port), _password(password) { }

Server::~Server(void) 
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        delete _users[i];
    }
}

const std::string &Server::getPort(void) const
{
    return (_port);
}

const std::string &Server::getPassword(void) const
{
    return (_password);
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
    freeaddrinfo(servinfo);
    std::cout << "Server Socket fd : " << _socket_fd << std::endl;
}

void    Server::loop(void)
{
    struct pollfd serv_fd; //struct pollfd => .events = parametres d'entree, .revents = parametres de sortie detecte
    serv_fd.fd = _socket_fd;
    serv_fd.events = POLLIN; //POLLIN = attente de lecture | POLLOUT = ecriture non-bloquante
    _fds.push_back(serv_fd); //vector<struct pollfd>

    std::cout << GREEN << "GREEN Message Client" << RESET << std::endl;
    std::cout << CYAN << "CYAN Reponse Server" << RESET << std::endl;

    while (1)
    {
        static int i;
        i++;
        std::cout << "\r(Poll " << i << ") " << std::flush;

        nfds_t n_fds = _fds.size(); //nfds_t = size_t pour pollfd
        if (poll(&_fds[0], n_fds, -1) == -1)
            throw std::runtime_error("Error server: poll");
        if (_fds[0].revents & POLLIN) //si un event POLLIN sur le serv
        {
            createUser();
            //displayAllUsers();
        }
        for (std::vector<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
        {
            if (i_pollfd->revents & POLLIN) //si un event POLLIN sur un user
            {
                userMsg(i_pollfd->fd);
                //displayAllUsers();
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
    for (std::vector<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
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
    if (fcntl(_fds.back().fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: fcntl()");
    /*int flags = fcntl(_fds.back().fd, F_GETFL, 0);
    if (flags & O_NONBLOCK)
        std::cout << "non_bloquant" << std::endl;
    else
        std::cout << "bloquant" << std::endl;*/
    std::cout << "User Connection fd " << _fds.back().fd << std::endl;

    User    *newuser = new User;

    _users.push_back(newuser);
    _users.back()->setFd(_fds.back().fd);
}

void Server::deleteUser(int user_fd)
{
    //enlever le user de la liste
    for (std::vector<struct pollfd>::iterator i_pollfd = _fds.begin() + 1; i_pollfd < _fds.end(); i_pollfd++)
    {
        if (i_pollfd->fd == user_fd)
        {
            _fds.erase(i_pollfd);
            std::cout << "User Deconnection fd : " << user_fd << std::endl;
            break;
        }
    }
    
    size_t i = 0;
    for (std::deque<User*>::iterator it = _users.begin(); it < _users.end(); it++)
    {
        if (_users[i]->getFd() == user_fd)
        {
            _users.erase(it);
            break;
        }
        i++;
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

void    Server::userMsg(int user_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t n = BUFFER_SIZE;
    std::string message;
    User *current_user;

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
            deleteUser(user_fd);
            return;
        }
        else
        {
            message.insert(message.length(), buffer, static_cast<size_t>(n));
        }
    }
    std::cout << "User fd " << user_fd << " : " << std::endl << GREEN;
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

    current_user = findUser(user_fd);
    current_user->setMessage(message);
    if (current_user->getLastChar() == '\n')
    {
        current_user->tokenizeMessage(current_user->getMessage());
        Server::execute(current_user);
        Server::displayAllChannels();
    }
    //std::cout << "message: " << current_user->getMessage() << std::endl;
    //std::cout << "lastchar: " << current_user->getLastChar() << ":" << std::endl;
    //std::cout << "nb tokens: " << current_user->getTokens().size() << std::endl;
}

void    Server::createChannel( User *user_creator, std::string name, std::string key )
{
    Channel    *newchannel = new Channel;

    newchannel->setOperator(user_creator);
    newchannel->setChannelName(name);
    newchannel->setChannelKey(key);
    _channels.push_back(newchannel);
}

void    Server::createChannel( User *user_creator, std::string name )
{
    Channel    *newchannel = new Channel;

    newchannel->setOperator(user_creator);
    newchannel->setChannelName(name);
    _channels.push_back(newchannel);
}

////////////////////////execution////////////////////////

void	Server::execute( User *current_user )
{
	std::string	commands[1] = {"JOIN"}; //, "KICK", "INVITE", "TOPIC", "MODE"}; //ajouter fonctions au jur et a mesure
	int	i = 0;

	if (current_user->getTokens().size() > 0)
		while (current_user->getTokens()[0] != commands[i] && i++ < 1);

	switch (i) //agrandir ce switch au fur et a mesure
	{
		case 0:
			join(this, current_user, current_user->getTokens());
			break;
		default:
			//throw exception?
			break;
	}
}

////////////////////////displays for testing////////////////////////
  
void    Server::displayAllUsers(void) const //fonction de test modifiable a volonte
{
    std::cout << "nº\tFD\t\tNickname\tUsername\tHost\t\tStatus\t\tLast Message" << std::endl;
    for (size_t i = 0; i < _users.size(); i++)
    {
        std::cout << i << "\t";
        std::cout << _users[i]->getFd() << "\t\t";
        std::cout << "\t\t";
        std::cout << "\t\t";
        std::cout << "\t\t";
        std::cout << "\t\t";
        std::cout << _users[i]->getMessage();
        std::cout << std::endl;
    }
}

void    Server::displayAllChannels(void) const //fonction de test modifiable a volonte
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        std::cout << i << "channel: " << _channels[i]->getChannelName() << std::endl;
    }
}