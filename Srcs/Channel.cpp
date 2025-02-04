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

Channel::Channel(std::string chan_name)
{
    memset(this, 0, sizeof(*this));
    name = chan_name;
}

Channel const &Channel::operator=(Channel const &obj)
{
    this->name = obj.name;
    return (*this);
}

std::string Channel::getPsswd() const
{
    return(psswd);
}

void Channel::setPsswd(std::string key)
{
    psswd = key;
}

std::string Channel::getName() const
{
    return (name);
}

std::vector<Client> &Channel::getClients() 
{
    return (this->clients);
}

std::vector<std::string>& Channel::getOperators()
{
    return(this->operators);
}

void Channel::setInviteOnly(bool flag)
{
    this->invite_only = flag;
}

void Channel::setClientLimit(int limit)
{
    this->client_limit = limit;
}

void Channel::setClients(Client& client)
{
    this->clients.push_back(client);
}

bool Channel::getInviteOnly() const
{
    return(this->invite_only);
}
int Channel::getClientLimit() const
{
    return(this->client_limit);
}
std::string Channel::getTopic() const
{
    return (this->topic);
}
void Channel::setTopic(std::string str)
{
    this->topic = str;
}