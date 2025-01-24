#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"

void handle_commands(std::string buff, Server server)
{
    std::string cmd = buff.substr(0, buff.find(' '));
    if(cmd == "JOIN")
        join(server);
    std::cout << "TEST " << server.getClients().begin()->second.getMessage();
    // if(cmd == "NICK")
    //     nick();
    // ...
    
}

void join(Server server)
{
    (void)server;
}