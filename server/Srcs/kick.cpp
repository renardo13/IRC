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



int Server::kick(Client &client, Command &cmd)
{
    std::string user = cmd.getArg()[0];
    std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
    if (client.getOperator(*chan) == chan->getOperators().end())
        return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName())));
    std::map<int, Client>::iterator clients_in_serv = getClient(user);
    if (clients_in_serv == getClients().end())
        return (sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), user)));
    std::vector<Client*>::iterator target_chan = chan->isClientInChan(clients_in_serv->second);
    if (target_chan == chan->getClients().end())
        return (sendMessageToClient(client, ERR_USERNOTINCHAN2(client.getNickname(), user, chan->getName())));
    sendMessageToClient(*(*target_chan), RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), (*target_chan)->getNickname(), cmd.getMsg()));
    sendMessageToEveryone(RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), (*target_chan)->getNickname(), cmd.getMsg()), chan->getName());
    std::vector<Client*>::iterator target_op = (*target_chan)->getOperator(*chan);
    if (target_op != chan->getOperators().end())
        chan->getOperators().erase(target_op);
    chan->getClients().erase(target_chan);
    return (0);
}
