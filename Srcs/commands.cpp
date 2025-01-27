#include "../Includes/Client.hpp"
// #include "../Includes/Channel.hpp"
#include <string>
#include<vector>

int sendMessageToClient(Client &client, std::string msg);

void handle_commands(std::string buff, Server& server, Client &client)
{
    (void)server;
    std::cout << buff << std::endl;
    std::string cmd = buff.substr(0, buff.find(' '));
    if(cmd == "JOIN")
        join(server, client);
    // if(cmd == "NICK")
    //     nick();
    // ...
    else
        sendMessageToClient(client, "Unknown command");
    client.setMessage("");
    client.setNBytes(0);
}

int getChannelIndex(std::vector<Channel>& channels, std::string name)
{
    std::vector<Channel>::iterator it = channels.begin();
    for(int i = 0; it != channels.end(); it++)
    {
        if(it->getName() == name)
            return(i);
        i++;
    }
    return(0);
}

void join(Server &server, Client &client)
{
    // server.print_clients();
    std::string mess = client.getMessage().substr(client.getMessage().find(' '));
    std::string name = mess.substr(mess.find_first_not_of(' '));
    if (!getChannelIndex(server.getChannels(), name))
    {
        client.setAdmin(1);
        server.getChannels().push_back(Channel(name));
    }
    std::cout << "NAME " << name << std::endl;
   (void)server;
}