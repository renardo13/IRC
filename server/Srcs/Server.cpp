#include "../Includes/Server.hpp"

Server::Server()
{
    clients = std::map<int, Client>();
    channels = std::vector<Channel>();
    motd_file = "./MOTD";
    pass = "";
    pfd_count = 0;
    memset(pfds, 0, sizeof(pfds));
}

Server::~Server()
{
}

Server::Server(std::map<int, Client>& clients, std::vector<Channel>& channels) : clients(clients), channels(channels)
{
    pass = "";
    pfd_count = 0;
    memset(pfds, 0, sizeof(pfds));
}

Server::Server(Server const &obj)
{
    if (this != &obj)
        *this = obj;
}

Server const &Server::operator=(Server const &obj)
{
    this->clients = obj.clients;
    return (*this);
}

std::map<int, Client> &Server::getClients()
{
    return (clients);
}

std::vector<Channel> &Server::getChannels()
{
    return (this->channels);
}

void Server::setClients(std::map<int, Client> &clients)
{
    this->clients = clients;
}

void Server::addNewClient(Client &client, int fd)
{
    this->clients.insert(std::pair<int, Client>(fd, client));
}

int Server::sendMessageToClient(Client &client, std::string msg)
{
    std::string irc_msg = msg + "\r\n";
    std::cout << BOLD << RED << "[SERVER] => " << msg << RESET << std::endl;
    return (send(client.getPfd(), irc_msg.c_str(), irc_msg.size(), 0));
}

void Server::setChannel(Channel &chan)
{
    this->channels.push_back(chan);
}

std::string Server::getPassword() const
{
    return (this->pass);
}

void Server::setPassword(std::string password)
{
    this->pass = password;
}

Client *Server::getOneClientByNickname(std::string nickname)
{
    std::map<int, Client>::iterator it = getClients().begin();
    for (; it != getClients().end(); it++)
    {
        if (it->second.getNickname() == nickname)
        {
            return (&(it->second));
        }
    }
    return (0);
}

std::map<int, Client>::iterator Server::getClient(Client& client)
{
    for(std::map<int, Client>::iterator it = getClients().begin(); it != getClients().end(); it++)
    {
        if(it->second.getNickname() == client.getNickname())
            return(it);
    }
    return(getClients().end());
}


std::map<int, Client>::iterator Server::getClient(std::string name)
{
    for(std::map<int, Client>::iterator it = getClients().begin(); it != getClients().end(); it++)
    {
        if(it->second.getNickname() == name)
            return(it);
    }
    return(getClients().end());
}

void Server::closeAllClientPfds()
{
    std::map<int, Client>::iterator it = getClients().begin();
    for (;it != getClients().end(); it++)
    {
        close(it->first);
    }
}
