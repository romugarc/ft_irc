#include "ft_irc.hpp"

extern bool g_pascommun;

static int verif_port(char *port)
{
	int	port_num;
	int i = 0;
	while (port[i] != '\0')
	{
		if (!isdigit(port[i]))
			return 0;
		i++;
	}
	if (i <= 0 || i >= 6)
		return 0;
	port_num = std::atoi(port);
	if (port_num < 1024 || port_num > 65535)
		return 0;
	return 1;
}

static int verif_password(char *password)
{
	(void)password;
	return 1;
}

static void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_pascommun = 1;
	}
}

int	main(int argc, char **argv)
{
	signal(SIGINT, &signal_handler);
	if (argc != 3)
    {
        std::cerr << "Error usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
	if (!verif_port(argv[1]))
	{
        std::cerr << "Error port" << std::endl;
        return 1;
	}
	if (!verif_password(argv[2]))
	{
        std::cerr << "Error password" << std::endl;
        return 1;
	}

	Server server(argv[1], argv[2]);

	try
	{
		server.createSocket();
		server.loop();
		server.deleteSocket();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}