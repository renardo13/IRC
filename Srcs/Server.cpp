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

void Server::setClients(std::map<int, Client> &clients)
{
    this->clients = clients;
}

void Server::print_clients(Server& server)
{
    std::map<int, Client>::iterator it = server.getClients().begin();

    for (; it != server.getClients().end(); it++)
    {
        std::cout << "Pfd: " << it->first << ", Client: " << it->second.getMessage() << std::endl;
    }
}