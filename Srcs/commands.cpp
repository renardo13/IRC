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
    Client &client = server.getClients()[fd];
    std::string msg = client.getMessage();
    std::string cmd = msg.substr(0, msg.find(' '));
    std::cout << cmd << std::endl;
    if (cmd == "PASS")
    {
        if (client.getRegisterProcess() == 0)
        {
            std::string pass = msg.substr(msg.find(' ') + 1);
            client.setRegisterProcess(1);
            //pass auth
        }
    }
    else if (cmd == "JOIN")
    {
        join(server, server.getClients()[fd]);
    }
    else if (cmd == "LEAVE")
    {
        leave(server, server.getClients()[fd]);
    }
    else if (cmd == "NICK")
    {
        if (client.getRegisterProcess() == 1)
        {
            std::string nick = msg.substr(msg.find(' ') + 1);
            client.setNickname(nick);
            client.setRegisterProcess(2);
        }
        else
        {
            client.setRegisterProcess(0);
        }  
    }
    else if (cmd == "USER")
    {
        if (client.getRegisterProcess() == 2)
        {
            std::string user = msg.substr(msg.find(' ') + 1);
            client.setUsername(user); 
            client.SetIsRegistered(true);
            client.setRegisterProcess(3);
            sendMessageToClient(client,getWelcomeMessage(client).c_str());
        }
        else
           client.setRegisterProcess(0); 
    }
    else
        sendMessageToClient(server.getClients()[fd], "Unknown command");
    client.setMessage("");
    client.setNBytes(0);
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