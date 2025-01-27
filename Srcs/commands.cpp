#include "../Includes/Client.hpp"
int sendMessageToClient(Client &client, std::string msg);

void handle_commands(std::string buff, Server& server, Client &client)
{
    (void)server;
    client.setMessage("");
    client.setNBytes(0);
    std::cout << buff << std::endl;
    sendMessageToClient(client, "Unknown command");
    // std::string cmd = buff.substr(0, buff.find(' '));
    // if(cmd == "JOIN")
    //     join();
    // if(cmd == "NICK")
    //     nick();
    // ...
}

void join(Server &server)
{

   (void)server;
}