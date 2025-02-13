#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

int Server::add_operator(Client &client, Command &cmd, Channel &chan)
{
    std::string user = cmd.getArg()[0];
    if (getClient(user) == getClients().end())
        return (sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), user)));
    std::vector<Client *>::iterator target = chan.isClientInChan(user);
    if ((*target)->getOperator(chan) == chan.getClients().end())
        return (sendMessageToClient(client, ERR_USERNOTINCHAN2(client.getNickname(), user, chan.getName())));
    chan.getOperators().push_back(*target);
    return (0);
}

int Server::remove_operator(Client &client, Command &cmd, Channel &chan)
{
    std::string user = cmd.getArg()[0];
    if (getClient(user) == getClients().end())
        return (sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), user)));
    std::vector<Client *>::iterator target = chan.isClientInChan(user);
    if ((*target)->getOperator(chan) == chan.getClients().end())
        return (sendMessageToClient(client, ERR_USERNOTINCHAN2(client.getNickname(), user, chan.getName())));
    std::vector<Client *>::iterator target_op = (*target)->getOperator(chan);
    if (target_op != chan.getOperators().end())
        chan.getOperators().erase(target_op);
    return (0);
}

int Server::set_limit(Channel &chan, Command &cmd)
{
    int limit = atoi(cmd.getArg()[0].c_str());
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
    std::vector<Channel>::iterator chan = isChannelInServer(cmd.getChannel()[0]);
    if (client.getOperator(*chan) == chan->getOperators().end() && chan->getTopicOp())
        return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName())));
    std::string arg;
    if (cmd.getArg().empty())
        arg = "";
    else
        arg = cmd.getArg()[0];
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
        remove_operator(client, cmd, *chan);
    else if (cmd.getMode()[0] == "+l")
        set_limit(*chan, cmd);
    else if (cmd.getMode()[0] == "-l")
        chan->setClientLimit(MAX_CLIENTS);
    else if (cmd.getMode()[0] == "+t")
        chan->setTopicOp(true);
    else if (cmd.getMode()[0] == "-t")
        chan->setTopicOp(false);
    else if (cmd.getMode()[0] == "+k")
        chan->setPsswd(cmd.getArg()[0]);
    else if (cmd.getMode()[0] == "-k")
        chan->setPsswd("");
    else if (!mode.empty())
        return (sendMessageToClient(client, ERR_UNKNOWNMODE(client.getNickname(), mode)));
    return (sendMessageToEveryone(RPL_CHANGEMODE(client.getHostname(), chan->getName(), cmd.getMode()[0], arg), chan->getName()));
}
