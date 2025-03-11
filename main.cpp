#include "Server.hpp"
#include "utils.hpp"

Server *global_server;

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Usage: ./a.out <port> <password>" << std::endl;
		return 1;
	}
 //   Server program(av[1], std::atoi(av[2]), "GreatestServerEver");
	try
	{
		validFormat(PASSWORD, av[2]);
		validFormat(PORT, av[1]);
		Server program(av[2], std::atoi(av[1]), "ft_irc");
		program.launch();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}