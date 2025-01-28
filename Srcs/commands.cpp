#include "../Includes/Client.hpp"
int sendMessageToClient(Client &client, std::string msg);

void handle_commands(Server& server, int fd)
{
    Client &client = server.getClients()[fd];
    std::string msg = client.getMessage();

    std::string cmd = msg.substr(0, msg.find(' '));
    std::cout << "CMD: " << cmd << "*" << std::endl;
    if (cmd == "PING")
    {
        std::cout << "PING ARRIVED" << std::endl;
    }
    if (cmd == "PASS")
    {
        std::cout << "****PASS" << std::endl;
    }
    if (cmd == "NICK")
    {
        std::string nick = msg.substr(msg.find(' ') + 1);
        client.setNickname(nick);
    }
    if (cmd == "USER")
    {
        std::string user = msg.substr(msg.find(' ') + 1);
        client.setUsername(user);
        sendMessageToClient(client,getWelcomeMessage(client).c_str());
    }
    if (cmd == "MODE")
    {
    }
    else
        sendMessageToClient(server.getClients()[fd], "Unknown command");
    server.getClients()[fd].setMessage("");
    server.getClients()[fd].setNBytes(0);
}

void join(Server &server)
{

   (void)server;
}