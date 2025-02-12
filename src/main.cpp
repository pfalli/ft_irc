#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
        throw std::out_of_range("Follow subject: ./ircserv <port> <password>");
    
    Server program;

    try
    {
        program;
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}