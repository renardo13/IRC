#include "../Includes/Client.hpp"
int sendMessageToClient(Client &client, std::string msg);

void handle_commands(Server& server, int fd)
{
    std::string msg = server.getClients()[fd].getMessage();
    std::string cmd = msg.substr(0, msg.find(' '));
    //std::cout << cmd << "MMMMMMMMMMMMMMMMMM;
    sendMessageToClient(server.getClients()[fd], "Unknown command");
    // if(cmd == "JOIN")
    //     join();
    // if(cmd == "NICK")
    //     nick();
    // ...
    server.getClients()[fd].setMessage("");
    server.getClients()[fd].setNBytes(0);
}

void join(Server &server)
{

   (void)server;
}