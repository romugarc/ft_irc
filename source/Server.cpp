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
    //////////////////////////faire setsockopt pour eviter l'erreur de bind
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
    std::cout << "New Server Socket fd : " << _socket_fd << std::endl;
}

void    Server::loop(void)
{
    struct pollfd serv_fd; //struct pollfd => .events = parametres d'entree, .revents = parametres de sortie detecte
    serv_fd.fd = _socket_fd;
    serv_fd.events = POLLIN; //POLLIN = attente de lecture | POLLOUT = ecriture non-bloquante
    _fds.push_back(serv_fd); //vector<struct pollfd>

    while (1)
    {
        nfds_t n_fds = _fds.size(); //nfds_t = size_t pour pollfd
        if (poll(&_fds[0], n_fds, -1) == -1)
            throw std::runtime_error("Error server: poll");
        if (_fds[0].revents & POLLIN) //si un event POLLIN sur le serv
        {
            createUser();
            //Server::displayAllUsers(); //fonction test affiche les users et leur contenu, fonction customisable
        }
        for (std::vector<struct pollfd>::iterator user_fd = _fds.begin() + 1; user_fd < _fds.end(); user_fd++)
        {
            if (user_fd->revents & POLLIN) //si un event POLLIN sur un user
            {
                userMsg(user_fd);
                //Server::displayAllUsers();
            }
        }
        //std::cout << "TEST LOOP" << std::endl;
   }
}

void    Server::deleteSocket(void)
{
    for (std::vector<struct pollfd>::iterator user_fd = _fds.begin() + 1; user_fd < _fds.end(); user_fd++)
    {
        if (close(user_fd->fd)) //close la socket de chaque user
            throw std::runtime_error("Error: close()");
        _fds.erase(user_fd); //erase le vecteur de chaque user
    }

    ////////////////////////////close les channel socket? -- Non, les channels n'ont pas de sockets

    if (close(_socket_fd)) //close la socket du server
        throw std::runtime_error("Error: close()");
}

void    Server::createUser(void)
{
    struct sockaddr_in sock_new_user; //sockaddr_in = ipv4 internet domain
    socklen_t sock_new_user_len = sizeof(sock_new_user);

    _fds.push_back(pollfd());
    _fds.back().events = POLLIN;
	_fds.back().fd = accept(_socket_fd, (struct sockaddr *)(&sock_new_user), &sock_new_user_len); //accepte la connexion user (POLLIN) et retourne la socket user
    if (_fds.back().fd == -1)
        throw std::runtime_error("Error: accept()");
    std::cout << "New User Socket fd : " << _fds.back().fd << std::endl;

    User    *newuser = new User;

    _users.push_back(newuser);
    _users.back()->setFd(_fds.back().fd);
}

void    Server::displayAllUsers(void) const //fonction de test modifiable a volonte
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        std::cout << i << ": " << _users[i]->getFd() << std::endl;
        std::cout << _users[i]->getMessage() << std::endl;
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

void    Server::userMsg(std::vector<struct pollfd>::iterator user_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t n = BUFFER_SIZE;
    std::string message;
    User *current_user;

    for (int j = 0; j < BUFFER_SIZE; j++)
        buffer[j] = 0;
    while (n == BUFFER_SIZE)
    {
        n = recv(user_fd->fd, buffer, BUFFER_SIZE, 0);
        if (n == -1)
            throw std::runtime_error("Error: recv()");
        else if (n == 0)
        {
            if (close(user_fd->fd))
                throw std::runtime_error("Error: close()");
            _fds.erase(user_fd);
        }
        else
        {
            message.insert(message.length(), buffer, static_cast<size_t>(n));
            current_user = findUser(user_fd->fd);
            current_user->setMessage(message);
            if (current_user->getLastChar() == '\n')
            {
                current_user->tokenizeMessage(current_user->getMessage());
                //current_user->execute(/*arg*/);
                //se concentrer sur join
            }
            // std::cout << "message: " << current_user->getMessage() << std::endl;
            // std::cout << "lastchar: " << current_user->getLastChar() << ":" << std::endl;
            // std::cout << "nb tokens: " << current_user->getTokens().size() << std::endl;
            // current_user->displayTokens();
            //Server::displayAllUsers();
            std::cout << "User (socket fd : " << user_fd->fd << " ) :" << message << std::endl;
        }
    }
}