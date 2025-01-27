#include "../Includes/Client.hpp"
#include <string>
#include <vector>

int sendMessageToClient(Client &client, std::string msg);

void handle_commands(Server &server, int fd)
{
    std::string msg = server.getClients()[fd].getMessage();
    std::string cmd = msg.substr(0, msg.find(' '));
    if (cmd == "JOIN")
    {
        join(server, server.getClients()[fd]);
    }
    if (cmd == "KICK")
    {
        join(server, server.getClients()[fd]);
    }
    if (cmd == "MODE")
    {
        join(server, server.getClients()[fd]);
    }
    if (cmd == "INVITE")
    {
        join(server, server.getClients()[fd]);
    }
    else
    {
        sendMessageToClient(server.getClients()[fd], "Unknown command");
        server.getClients()[fd].setMessage("");
        server.getClients()[fd].setNBytes(0);
    }
}

// // generic template function to find a specific attribute in a class
// template <typename Container, typename Value, typename AttributeType>
// typename Container::iterator findByAttribute(Container &container, Value value, AttributeType Container::value_type::*attribute)
// {
//     typename Container::iterator it = container.begin();
//     for (; it != container.end(); ++it)
//     {
//         if (it->*attribute == value)
//         {
//             return it;
//         }
//     }
//     return container.end();
// }

template <typename Container, typename Value>
typename Container::iterator findByAttribute(Container &container, bool (*comparator)(const typename Container::value_type&))
{
    typename Container::iterator it = container.begin();
    for (; it != container.end(); ++it)
    {
        if (comparator(*it))  // Utilisation du pointeur de fonction pour comparer
        {
            return it;
        }
    }
    return container.end();
}

std::string compareChannelName(const Channel &chan)
{
    return chan.getName();
}


void join(Server &server, Client &client)
{

    std::string mess = client.getMessage().substr(client.getMessage().find(' '));
    std::string name = mess.substr(mess.find_first_not_of(' '));
    if (name.empty())
        std::cout << "Missing channel name\n";
    else
    {
        Channel chan(name);
        chan.getClients().push_back(client);
        if (findByAttribute(server.getChannels(), compareChannelName) != server.getChannels().end())
        {
            server.getChannels().push_back(chan);
            client.getChannel().push_back(chan);
            client.setAdmin(1);
            std::cout << "Channel was successfully created\n";
        }
        else if (name == "0")
        {
            client.getChannel().clear();
            std::cout << "Erase of the channels-s client\n";
        }
        else
        {
            client.getChannel().push_back(chan);
            std::cout << "Client reached successfully an existing channel\n";
        }
    }
    // server.print_clients();
}
