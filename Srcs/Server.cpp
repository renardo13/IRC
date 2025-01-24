#include "../Includes/Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

Server::Server(std::map<int, Client> clients) : clients(clients)
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
