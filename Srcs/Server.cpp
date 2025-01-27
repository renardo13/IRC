#include "../Includes/Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

Server::Server(std::map<int, Client>& clients) : clients(clients)
{
}

Server::Server(Server const& obj)
{
    if(this != &obj)
        *this = obj;
}

Server const& Server::operator=(Server const& obj)
{
    this->clients = obj.clients;
    return(*this);
}

std::map<int, Client>& Server::getClients()
{
    return(clients);
}

std::vector<Channel>& Server::getChannels()
{
    return(this->channels);
}

void Server::setClients(std::map<int, Client> &clients)
{
    this->clients = clients;
}

void Server::print_clients()
{
    std::map<int, Client>::iterator it = getClients().begin();

    for (int i = 1; it != getClients().end(); it++)
    {
        std::cout << std::string(40, '_');
        std::cout << "\nClient number " << i << std::endl;
        i++;
        std::cout << "Pfd: " << it->first << ", Client message: " << it->second.getMessage() << std::endl;
        std::vector<Channel>::iterator chan_it = it->second.getChannel().begin();
        for(; chan_it != it->second.getChannel().end(); chan_it++)
        {
           std::cout << chan_it->getName();
           std::cout << std::endl;
        }
    }
}

void Server::addNewClient(Client &client, int fd)
{
    this->clients.insert(std::pair<int, Client>(fd, client));
}