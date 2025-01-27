#include "../Includes/Client.hpp"
#include <string>
#include <vector>

int sendMessageToClient(Client &client, std::string msg);

void handle_commands(Server &server, int fd)
{
    std::string msg = server.getClients()[fd].getMessage();
    std::string cmd = msg.substr(0, msg.find(' '));
    std::cout << server.getClients()[fd].getMessage();
    if (cmd == "JOIN")
    {
        std::cout << "hy\n";
        join(server, server.getClients()[fd]);
    }
    else
    {
        sendMessageToClient(server.getClients()[fd], "Unknown command");
        server.getClients()[fd].setMessage("");
        server.getClients()[fd].setNBytes(0);
    }
}

int getChannelIndex(std::vector<Channel> &channels, std::string name)
{
    std::vector<Channel>::iterator it = channels.begin();
    for (int i = 0; it != channels.end(); it++)
    {
        if (it->getName() == name)
            return (i);
        i++;
    }
    return (0);
}

void join(Server &server, Client &client)
{
    
    std::string mess = client.getMessage().substr(client.getMessage().find(' '));
    std::string name = mess.substr(mess.find_first_not_of(' '));
    if (!getChannelIndex(server.getChannels(), name))
    {
        Channel chan(name);
        server.getChannels().push_back(chan);
        client.getChannel().push_back(chan);
        client.setAdmin(1);
    }
    server.print_clients();
}