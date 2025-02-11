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
        std::cout << "Invalid arguments, please enter a port and a password ./ircserv <port> <passeword>\n";
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
