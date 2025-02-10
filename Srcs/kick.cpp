#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

int Server::kick(Client &client, Command &cmd)
{
    Channel* chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
    if (!client.is_operator(*chan))
        return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName())));
    std::map<int, Client>::iterator clients_in_serv = getClients().begin();
    for (; clients_in_serv != getClients().end(); clients_in_serv++)
    {
        if (clients_in_serv->second.getNickname() == cmd.getUser()[0])
            break;
    }
    if (clients_in_serv == getClients().end())
        return (sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), cmd.getUser()[0])));
    Client* target = findValue(chan->getClients(), &Client::getNickname, cmd.getUser()[0]);
    if (target != NULL)
        return (sendMessageToClient(client, ERR_USERNOTINCHAN2(client.getNickname(), cmd.getUser()[0], cmd.getChannel()[0])));
    if (target->is_operator(*chan))
        chan->getOperators().erase(std::find(chan->getOperators().begin(), chan->getOperators().end(), cmd.getUser()[0]));
    sendMessageToEveryone(RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), target->getNickname(), cmd.getMsg()), chan->getName());
    sendMessageToClient(*target, RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), target->getNickname(), cmd.getMsg()));
    chan->getClients().erase(std::find(chan->getClients().begin(), chan->getClients().end(), cmd.getUser()[0]));
    return (0);
}
