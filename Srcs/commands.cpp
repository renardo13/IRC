#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include <stdio.h>

void handle_commands(std::string buff, Server &server)
{
    std::string cmd = buff.substr(0, buff.find(' '));
    if (cmd == "JOIN")
    {
        std::cout << "hello" << std::endl;
        
        join(server);
    }
    // if(cmd == "NICK")
    //     nick();
    // ...
}

void join(Server &server)
{

   (void)server;
}