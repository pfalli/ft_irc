#include "Server.hpp"
#include "utils.hpp"


int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Usage: ./a.out <password> <port>" << std::endl;
		return 1;
	}
 //   Server program(av[1], std::atoi(av[2]), "GreatestServerEver");
	try
	{
		validFormat(PASSWORD, av[1]);
		validFormat(PORT, av[2]);
	    Server program(av[1], std::atoi(av[2]), "GreatestServerEver");
	    program.launch();
	    return 0;
	}
	catch (const std::exception& e)
	{
	    std::cerr << e.what() << std::endl;
	    return 1;
	}
}