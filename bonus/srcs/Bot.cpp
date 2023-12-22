/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcharpen <tcharpen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 12:17:35 by tcharpen          #+#    #+#             */
/*   Updated: 2023/08/09 12:17:35 by tcharpen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

bool	sig = false;

Bot::Bot(void) { }

Bot::Bot(const Bot &src) { *this = src; }

Bot::Bot(std::string addr, const char *port, std::string pass, std::string name) : _addr(addr), _pass(pass), _name(name), _msg_rcv("")
{
	_param_sock.ai_family = AF_INET;
    _param_sock.ai_socktype = SOCK_STREAM;
    _param_sock.ai_flags = AI_PASSIVE;
    _param_sock.ai_protocol = IPPROTO_TCP;
	_port = atoi(port);
	if (getaddrinfo(_addr.c_str(), port, &_param_sock, &_servinfo) < 0)
        throw std::runtime_error("Error: getaddrinfo()");
	_fd = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
    if (_fd == -1)
    {
        freeaddrinfo(_servinfo);
        throw std::runtime_error("Error: socket()");
    }
	int optvalue = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue)) < 0)
    {
        freeaddrinfo(_servinfo);
		close(_fd);
        throw std::runtime_error("Error: setsockopt()");
    }
	while (connect(_fd, _servinfo->ai_addr, _servinfo->ai_addrlen) < 0)
    {
		if (sig == true)
		{
			freeaddrinfo(_servinfo);
			close(_fd);
			throw std::runtime_error("Error: connect()");
		}
        std::cerr << "connect() failed" << std::endl;
		std::cerr << "Retrying..." << std::endl;
		sleep(3);
    }
	freeaddrinfo(_servinfo);
    _servinfo = NULL;
}

Bot::~Bot(void) { }

Bot	&Bot::operator=(const Bot &src)
{
	this->_param_sock = src._param_sock;
	this->_servinfo = src._servinfo;
	this->_addr = src._addr;
	this->_pass = src._pass;
	this->_name = src._name;
	this->_port = src._port;
	this->_fd = src._fd;
	this->_msg_rcv = src._msg_rcv;
	return (*this);
}

void	Bot::send_msg(std::string msg)
{
	std::string	message;

	message = msg + "\r\n";
	if (send(_fd, message.c_str(), message.size(), 0) == -1)
	{
		close(_fd);
		throw std::runtime_error("Error: send()");
	}
}

bool	Bot::send_nick(void)
{
    size_t      pos;
    std::string nickname;

	send_msg("PASS " + _pass + "\r\n" + "NICK " + _name);
	sleep(1);
    if (!recv_msg())
		return (false);
    if (_msg_rcv.find(" 432 ") != std::string::npos
		|| _msg_rcv.find(" 464 ") != std::string::npos
		|| _msg_rcv.find(" 431 ") != std::string::npos
		|| _msg_rcv.find(" 461 ") != std::string::npos)
    {
		close(_fd);
		if (_msg_rcv.find(" 461 ") != std::string::npos
			|| _msg_rcv.find(" 464 ") != std::string::npos)
			std::cerr << "Password incorrect" << std::endl;
		else if (_msg_rcv.find(" 431 ") != std::string::npos
			|| _msg_rcv.find(" 432 ") != std::string::npos)
        	std::cerr << "Erroneous name" << std::endl;
        return (false);
    }
    pos = _msg_rcv.find("NICK");
    nickname = _msg_rcv.substr(pos + 5, std::string::npos);
    if (nickname != _name)
    {
        close(_fd);
        std::cerr << "Nickname already in use" << std::endl;
        return (false);
    }
	return (true);
}

bool	Bot::recv_msg(void)
{
    char            buf[BUFFER_SIZE];
	struct pollfd   pfd;
    int             read_count;

	_msg_rcv = "";
	pfd.fd = _fd;
	pfd.events = POLLIN;
	if (poll(&pfd, 1, -1) <= 0)
	{
		close(_fd);
		if (errno == EINTR)
            return (false);
        else
            throw std::runtime_error("Error: poll()");
	}
	if (pfd.revents != POLLIN)
		return (true);
	read_count = recv(_fd, buf, BUFFER_SIZE, 0);
	if (read_count < 0)
	{
		close(_fd);
		throw std::runtime_error("Error: recv()");
	}
	else if (read_count == 0)
    {
		close(_fd);
        std::cout << "Connexion closed !" << std::endl;
        return (false);
    }
	for (int i = 0; i < read_count ; i++)
		_msg_rcv += buf[i];
	while (_msg_rcv.find("\r\n") != std::string::npos)
		_msg_rcv.erase(_msg_rcv.find("\r\n"), 2);
	return (true);
}

void	Bot::check_msg(void)
{
	bool chan_exist = false;
	std::string chan = "";
	std::string	nick;
	std::string	user;
	std::string	host;
	std::string	msg;

	if (_msg_rcv.find(" INVITE ") != std::string::npos)
	{
		chan = _msg_rcv.substr(_msg_rcv.find(" INVITE ") + 9 + _name.size());
		for (std::deque<std::string>::iterator it = _chans.begin(); it < _chans.end(); it++)
		{
			if (!strcmp(chan.c_str(), (*it).c_str()))
				chan_exist = true;
		}
		if (chan_exist == false)
		{
			send_msg("JOIN " + chan);
		}
	}
	else if (_msg_rcv.find(" JOIN ") != std::string::npos)
	{
		chan = _msg_rcv.substr(_msg_rcv.find(" JOIN ") + 6);
		chan = chan.substr(0, chan.find(":"));
		_chans.push_back(chan);
		std::cout << "Joined channel " << chan << std::endl;
	}
	else if (_msg_rcv.find(" KICK ") != std::string::npos)
	{
		for (std::deque<std::string>::iterator it = _chans.begin(); it < _chans.end(); it++)
		{
			chan = _msg_rcv.substr(_msg_rcv.find(" KICK ") + 6);
			chan = chan.substr(0, chan.find(" "));
			if (!strcmp(chan.c_str(), (*it).c_str()))
			{
				_chans.erase(it);
				std::cout << "Left channel " << chan << std::endl;
			}
		}
	}
	else if (_msg_rcv.find(" PRIVMSG ") != std::string::npos)
	{
		for (std::deque<std::string>::iterator it = _chans.begin(); it < _chans.end(); it++)
		{
			chan = _msg_rcv.substr(_msg_rcv.find(" PRIVMSG ") + 9);
			chan = chan.substr(0, chan.find(" "));
			if (!strcmp(chan.c_str(), (*it).c_str()))
			{
				nick = _msg_rcv.substr(1, _msg_rcv.find('!') - 1);
				user = _msg_rcv.substr(_msg_rcv.find('!') + 1);
				user = user.substr(0, user.find('@'));
				host = _msg_rcv.substr(_msg_rcv.find('@') + 1);
				host = host.substr(0, host.find(' '));
				msg = _msg_rcv.substr(_msg_rcv.find(" PRIVMSG ") + 11 + chan.size());
				std::cout << "User " << user << " a dit \"" << msg << "\" dans le channel " << chan << std::endl;
				if (msg == "!info")
				send_msg("NOTICE " + chan + " :Your nickname is " + nick + ", Your username is " + user + ", Your hostname is " + host);
			}
		}
	}
	/*if (cmd == "!roll")
		send_msg("NOTICE " + dest + " :" + ft_itoa(rand() % 6 + 1));
	else if (cmd == "!nick")
		send_msg("NOTICE " + dest + " :Your nickname is " + dest);
	else if (cmd == "!ping")
		send_msg("NOTICE " + dest + " :PONG");
	else if (cmd == "!youtube")
		send_msg("NOTICE " + dest + " :https://www.youtube.com/watch?v=dQw4w9WgXcQ");
	else if (cmd == "!help")
		send_msg("NOTICE " + dest + " :!roll - !nick - !ping - !youtube");*/
}

void    Bot::run(void)
{
	if (!send_nick())
		return ;
	send_msg("USER " + _name + " 0 * :" + _name);
    std::cout << "Connected as " << _name << std::endl;
    while (!sig)
    {
        if (!recv_msg())
			return ;
        if (_msg_rcv.find(" 001 " + _name + " :Welcome to the IRC 42 Network, ") != std::string::npos)
            break ;
    }
    std::cout << "Joined server " << _addr << std::endl;
    while (!sig)
    {
		if (!recv_msg())
			return ;
		check_msg();
    }
	close(_fd);
}

void	Bot::handler(int)
{
	sig = true;
}
