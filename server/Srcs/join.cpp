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


std::vector<Client*>::iterator Channel::isClientInChan(Client& client)
{
    for(std::vector<Client*>::iterator it = getClients().begin(); it != getClients().end(); it++)
    {
        if((*it)->getNickname() == client.getNickname())
            return(it);
    }
    return(getClients().end());
}


std::vector<Client*>::iterator Channel::isClientInChan(std::string name)
{
    for(std::vector<Client*>::iterator it = getClients().begin(); it != getClients().end(); it++)
    {
        if((*it)->getNickname() == name)
            return(it);
    }
    return(getClients().end());
}

int Server::reach_channel(Client &client, Command &cmd, Channel &chan, std::string chan_name)
{
    if (chan.isClientInChan(client) != chan.getClients().end())
        return (0);
    if ((int)chan.getClients().size() >= chan.getClientLimit())
        return (sendMessageToClient(client, ERR_TOOMANYCLIENTS(client.getNickname(), chan.getName())));
    else if (client.getNbChannels() > 10)
        return (sendMessageToClient(client, ERR_TOOMANYCHANNELS(client.getNickname(), chan.getName())));
    else if (chan.getInviteOnly() && (chan.getInviteOnly() && !chan.isClientInInviteList(client.getNickname())))
        return (sendMessageToClient(client, INVITE_ONLY(client.getNickname(), chan.getName())));
    else if (!chan.getPsswd().empty())
    {
        if (cmd.getArg().empty())
            return (sendMessageToClient(client, ERR_PSSWD(client.getNickname(), chan.getName())));
        if (cmd.getArg()[0] != chan.getPsswd())
            return (sendMessageToClient(client, ERR_PSSWD(client.getNickname(), chan.getName())));
    }
    client.IncreaseNbChannels();
    sendMessageToEveryone(RPL_JOIN(client.getHostname(), chan_name), chan_name);
    chan.getClients().push_back(&client);
    if (chan.getTopic().empty())
        sendMessageToClient(client, RPL_JOIN(client.getHostname(), chan_name) +
                                        RPL_NAMES(client.getNickname(), chan_name, getClientsList(chan)) +
                                        RPL_ENDOFNAMES(client.getNickname(), chan_name));
    else
        sendMessageToClient(client, RPL_JOIN(client.getHostname(), chan_name) +
                                        RPL_TOPIC(client.getNickname(), chan_name, chan.getTopic()) +
                                        RPL_NAMES(client.getNickname(), chan_name, getClientsList(chan)) +
                                        RPL_ENDOFNAMES(client.getNickname(), chan_name));
    return (0);
}

void Server::create_channel(Client &client, std::string chan_name)
{
    if (client.getNbChannels() > 10)
        sendMessageToClient(client, ERR_TOOMANYCHANNELS(client.getNickname(), chan_name));
    else
    {
        Channel channel(chan_name);
        channel.getClients().push_back(&client);
        channel.getOperators().push_back(&client);
        channel.setClientLimit(MAX_CLIENTS);
        setChannel(channel);
        sendMessageToClient(client, RPL_JOIN(client.getHostname(), chan_name) +
                                        RPL_NAMES(client.getNickname(), chan_name, '@' + client.getNickname()) +
                                        RPL_ENDOFNAMES(client.getNickname(), chan_name));
        client.IncreaseNbChannels();
    }
}

std::vector<Channel>::iterator Server::isChannelInServer(std::string chan)
{
    for(std::vector<Channel>::iterator it = getChannels().begin(); it != getChannels().end(); it++)
    {
        if(it->getName() == chan)
            return(it);
    }
    return(getChannels().end());
}

int Server::join(Client &client, Command &cmd)
{
    if (cmd.getChannel().empty())
        return (sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd())));
    std::vector<std::string>::iterator chan_name = cmd.getChannel().begin();
    for (; chan_name != cmd.getChannel().end(); chan_name++)
    {
        std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
        if(chan != getChannels().end())
            reach_channel(client, cmd, *chan, *chan_name);
        else
            create_channel(client, *chan_name);
    }
    return (0);
}