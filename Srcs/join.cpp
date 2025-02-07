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

int Server::reach_channel(Client &client, Command &cmd, Channel &chan, std::string chan_name)
{
    if (findValue(chan.getClients(), &Client::getNickname, client.getNickname()) != chan.getClients().end())
        return (0);
    if ((int)chan.getClients().size() >= chan.getClientLimit())
        return (sendMessageToClient(client, ERR_TOOMANYCLIENTS(client.getNickname(), chan.getName())));
    else if (client.getNbChannels() > 10)
        return (sendMessageToClient(client, ERR_TOOMANYCHANNELS(client.getNickname(), chan.getName())));
    else if (chan.getInviteOnly())
        return (sendMessageToClient(client, INVITE_ONLY(client.getNickname(), chan.getName())));
    else if (!chan.getPsswd().empty())
    {
        if (cmd.getUser().empty())
            return (sendMessageToClient(client, ERR_PSSWD(client.getNickname(), chan.getName())));
        if (cmd.getUser()[0] != chan.getPsswd())
            return (sendMessageToClient(client, ERR_PSSWD(client.getNickname(), chan.getName())));
    }
    client.IncreaseNbChannels();
    sendMessageToEveryone(RPL_JOIN(client.getHostname(), chan_name), chan_name);
    chan.getClients().push_back(client);
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
        setChannel(channel);
        getChannels().back().getClients().push_back(client);
        Client& admin = getChannels().back().getClients().back();
        getChannels().back().getOperators().push_back(admin);
        // getChannels().back().getOperators().push_back(client);
        getChannels().back().setClientLimit(MAX_CLIENTS);
        sendMessageToClient(client, RPL_JOIN(client.getHostname(), chan_name) +
                                        RPL_NAMES(client.getNickname(), chan_name, '@' + client.getNickname()) +
                                        RPL_ENDOFNAMES(client.getNickname(), chan_name));
        client.IncreaseNbChannels();
    }
}

int Server::join(Client &client, Command &cmd)
{
    if (cmd.getChannel().empty())
        return (sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd())));
    std::vector<std::string>::iterator chan_name = cmd.getChannel().begin();
    for (; chan_name != cmd.getChannel().end(); chan_name++)
    {
        std::vector<Channel>::iterator chan_find = findValue(getChannels(), &Channel::getName, *chan_name);
        if (chan_find != getChannels().end())
            reach_channel(client, cmd, *chan_find, *chan_name);
        else
            create_channel(client, *chan_name);
    }
    return (0);
}