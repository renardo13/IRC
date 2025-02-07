#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

// Generic function to find a value of a certain type in a certain container type. ("::Value_type" is the type of elements that the container store)
template <typename Container, typename AttributeType,
          typename Value>
typename Container::iterator findValue(Container &container,
                                       AttributeType (Container::value_type::*getter)() const, const Value value)
{
    typename Container::iterator it = container.begin();
    for (; it != container.end(); it++)
    {
        if (((*it).*getter)() == value)
            return (it);
    }
    return (container.end());
}
Client *client = (it != container.end()) ? &(*it) : nullptr;


int Server::add_operator(Client &client, Command &cmd, Channel &chan)
{
    std::vector<Client>::iterator target = findValue(chan.getClients(), &Client::getNickname, cmd.getUser()[0]);
    if (target == chan.getClients().end())
        return (sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), cmd.getUser()[0])));
    if (!target->is_operator(chan))
        chan.getOperators().push_back(*target);
    return (0);
}

int Server::remove_operator(Command &cmd, Channel &chan)
{
    std::vector<Client>::iterator target = findValue(chan.getClients(), &Client::getNickname, cmd.getUser()[0]);
    if (target->is_operator(chan))
        chan.getOperators().erase(target);
    return (0);
}

int Server::set_limit(Channel &chan, Command &cmd)
{
    int limit = atoi(cmd.getUser()[0].c_str());
    if (limit <= 1)
        limit = 1;
    if (limit > 512)
        limit = 512;
    chan.setClientLimit(limit);
    return (0);
}

int Server::mode(Client &client, Command &cmd)
{
    if (cmd.getChannel().empty() || cmd.getMode().empty())
        return (-1);
    std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
    if (!client.is_operator(*chan))
        return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName())));
    std::string arg;
    if (cmd.getUser().empty())
        arg = "";
    else
        arg = cmd.getUser()[0];
    std::string mode = cmd.getMode()[0];
    if (arg == "" && (mode == "+k" || mode == "+o" || mode == "-o" || mode == "+l"))
        return (sendMessageToClient(client, ERR_NEEDMOREPARAMSMODE(client.getNickname(), cmd.getMode()[0])));
    if (cmd.getMode()[0] == "+i" && !chan->getInviteOnly())
        chan->setInviteOnly(1);
    else if (cmd.getMode()[0] == "-i" && chan->getInviteOnly())
        chan->setInviteOnly(0);
    else if (cmd.getMode()[0] == "+o")
        add_operator(client, cmd, *chan);
    else if (cmd.getMode()[0] == "-o")
        remove_operator(cmd, *chan);
    else if (cmd.getMode()[0] == "+l")
        set_limit(*chan, cmd);
    else if (cmd.getMode()[0] == "-l")
        chan->setClientLimit(MAX_CLIENTS);
    else if (cmd.getMode()[0] == "+t")
        chan->setTopicOp(true);
    else if (cmd.getMode()[0] == "-t")
        chan->setTopicOp(false);
    else if (cmd.getMode()[0] == "+k")
        chan->setPsswd(cmd.getUser()[0]);
    else if (cmd.getMode()[0] == "-k")
        chan->setPsswd("");
    else if (!mode.empty())
        return (sendMessageToClient(client, ERR_UNKNOWNMODE(client.getNickname(), mode)));
    return (sendMessageToEveryone(RPL_CHANGEMODE(client.getHostname(), chan->getName(), cmd.getMode()[0], arg), chan->getName()));
}
