#include "../Includes/Channel.hpp"

Channel::Channel()
{
    topic = "";
    client_limit = MAX_CLIENTS;
    topic_op = true;
    psswd = "";
    invite_only = 0;
    clients = std::vector<Client>();
    operators = std::vector<std::string>();
}

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

Channel::Channel(std::string chan_name) : name(chan_name)
{
    topic = "";
    client_limit = MAX_CLIENTS;
    topic_op = true;
    psswd = "";
    invite_only = 0;
    clients = std::vector<Client>();
    operators = std::vector<std::string>();
}

Channel const &Channel::operator=(Channel const &obj)
{
    name = obj.name;
    topic_op = obj.topic_op;
    topic = obj.topic;
    return (*this);
}

std::string Channel::getPsswd() const
{
    return (psswd);
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

std::vector<std::string> &Channel::getOperators()
{
    return (this->operators);
}

void Channel::setInviteOnly(bool flag)
{
    this->invite_only = flag;
}

void Channel::setClientLimit(int limit)
{
    this->client_limit = limit;
}

void Channel::setClients(Client &client)
{
    this->clients.push_back(client);
}

bool Channel::getInviteOnly() const
{
    return (this->invite_only);
}
int Channel::getClientLimit() const
{
    return (this->client_limit);
}
std::string Channel::getTopic() const
{
    return (this->topic);
}
void Channel::setTopic(std::string str)
{
    this->topic = str;
}

void Channel::setTopicOp(bool flag)
{
    this->topic_op = flag;
}

bool Channel::getTopicOp() const
{
    return (this->topic_op);
}