#include "../Includes/Channel.hpp"

Channel::Channel() {}

Channel::~Channel()
{
}

Channel::Channel(Channel const &obj)
{
    if (this != &obj)
    {
        *this = obj;
    }
}

Channel::Channel(std::string name) : name(name)
{
}

Channel const &Channel::operator=(Channel const &obj)
{
    this->name = obj.name;
    this->mode = obj.mode;
    return (*this);
}

std::string Channel::getMode() const
{
    return (this->mode);
}

void Channel::setMode(std::string mode)
{
    this->mode = mode;
}

std::string Channel::getName() const
{
    return (name);
}

std::vector<Client> &Channel::getClients()
{
    return (this->clients);
}

void Channel::setClients(Client& client)
{
    this->clients.push_back(client);
}
