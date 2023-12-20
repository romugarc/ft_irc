#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <string>
#include <sys/socket.h> //socket(), bind(), listen()
#include <sys/types.h>
#include <netdb.h> //struct addrinfo   getnameinfo
#include <unistd.h> //close()
#include <poll.h> //poll(), struct pollfd
#include <vector>
#include <deque>
#include <fcntl.h>
#include <sstream> //stringstream

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Rep.hpp"
#include "Commands.hpp"

# define RESET "\033[0m"
# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"

#define BUFFER_SIZE 42

#endif