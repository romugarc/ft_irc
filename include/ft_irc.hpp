#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <string>
#include <sys/socket.h> //socket(), bind(), listen()
#include <sys/types.h>
#include <netdb.h> //struct addrinfo
#include <unistd.h> //close()
#include <poll.h> //poll(), struct pollfd
#include <vector>

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"

#define BUFFER_SIZE 42

#endif