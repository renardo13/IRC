#include "Client.hpp"
#include "Server.hpp"

int main(int ac, char **av)
{
    if(ac != 2)
    {
        std::cout << "Invalid arguments, please enter a port and a password ./ircserv <port> <passeword>\n";
    }
    try
    { 
        set_server(av[1], av[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
