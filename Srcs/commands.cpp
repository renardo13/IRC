#include "../Includes/Client.hpp"
#include <string>
#include <vector>

// Generic function to find a value of a certain type in a certain container type. ("::Value_type" is the type of elements that the container store)
template <typename Container, typename AttributeType, typename Value>
typename Container::iterator findValue(Container &container, AttributeType (Container::value_type::*getter)() const, const Value value)
{
    typename Container::iterator it = container.begin();

    for (; it != container.end(); it++)
    {
        // std::cout << "it : " << ((*it).*getter)() << " | value : " << value << std::endl;
        if (((*it).*getter)() == value)
            return it;
    }

    return container.end();
}

int sendMessageToClient(Client &client, std::string msg);

void handle_commands(Server &server, int fd)
{
    std::string msg = server.getClients()[fd].getMessage();
    std::string cmd = msg.substr(0, msg.find(' '));
    if (cmd == "JOIN")
    {
        join(server, server.getClients()[fd]);
    }
    if (cmd == "LEAVE")
    {
        leave(server, server.getClients()[fd]);
    }
    else if (cmd == "KICK")
    {
        join(server, server.getClients()[fd]);
    }
    else if (cmd == "MODE")
    {
        join(server, server.getClients()[fd]);
    }
    else if (cmd == "INVITE")
    {
        join(server, server.getClients()[fd]);
    }
    else
    {
        sendMessageToClient(server.getClients()[fd], "Unknown command");
    }
    server.getClients()[fd].setMessage("");
    server.getClients()[fd].setNBytes(0);
}

void join(Server &server, Client &client)
{

    std::string mess = client.getMessage().substr(client.getMessage().find(' '));
    std::string name = mess.substr(mess.find_first_not_of(' '));
    if (name.empty() || name[0] != '#')
    {
        std::cout << "Channel name is invalid\n";
        return;
    }
    name = name.substr(1);
    Channel chan(name);
    if (name == "0")
    {
        client.getChannel().clear();
        std::cout << "Erase all the channels's client\n";
    }
    else if (server.getChannels().size() == 0 || findValue(server.getChannels(), &Channel::getName, name) == server.getChannels().end())
    {
        chan.getClients().push_back(client);
        // std::cout << "Client nickname : " << chan.getClients().begin()
        server.getChannels().push_back(chan);
        client.getChannel().push_back(chan);
        client.setAdmin(1);
        std::cout << "Channel was successfully created\n";
    }
    else if (findValue(client.getChannel(), &Channel::getName, name) == client.getChannel().end())
    {
        chan.getClients().push_back(client);
        client.getChannel().push_back(chan);
        std::cout << "Client reached successfully an existing channel\n";
    }
    // server.print_clients();
}

void leave(Server &server, Client &client)
{
    std::string mess = client.getMessage().substr(client.getMessage().find(' '));
    std::string chan_name = mess.substr(mess.find_first_not_of(' '));

    // std::cout << "Channel name = " << chan_name << std::endl;

    std::vector<Channel>::iterator it = findValue(client.getChannel(), &Channel::getName, chan_name);

    std::vector<Client>::iterator client_it = it->getClients().begin();
    std::cout << "List of client inside this specific channel :\n";
    for(; client_it != it->getClients().end(); client_it++)
    {
        std::cout << "Client_name = " << client_it->getNickname() << std::endl;
    }

    if (it == client.getChannel().end())
    {
        std::cout << "Client is not in this channel\n";
    } 
    else
    {
        // it->getClients().erase(findValue(it->getClients(), &Client::getNickname, client.getNickname()));
        std::cout << "Client leave the channel\n";
        if (it->getClients().size() == 0)
        {
            server.getChannels().erase(findValue(server.getChannels(), &Channel::getName, chan_name));
            std::cout << "Channel was suppressed\n";
        }
        client.getChannel().erase(it);
    }
    server.print();
}