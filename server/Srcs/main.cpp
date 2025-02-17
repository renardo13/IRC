#include "Client.hpp"
#include "Server.hpp"
#include <csignal>

int isRunning = 0;

void signalHandler(int s_num)
{
    (void)s_num;
    isRunning = 0;
}

int main(int ac, char **av)
{
    signal(SIGINT, signalHandler);
    if(ac != 3)
    {
        std::cout << "Invalid arguments, please enter a port and a password ./ircserv <port> <passeword>\n";
        return 0;
    }
    if (atoi(av[1]) < 1024 || atoi(av[1]) > 65535)
    {
        std::cout << "Invalid port number, try to write it in range 1024 - 65535" << std::endl;
        return 0;
    }
    try
    {
        Server server;
        isRunning = 1;
        server.set_server(av[1], av[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 1;
}
