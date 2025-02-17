#include "../Includes/Client.hpp"
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

int Server::part(Client &client, Command &cmd)
{
	if(cmd.getChannel().empty())
		return (sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd())));
	std::vector<Channel>::iterator chan = findValue(getChannels(),
													&Channel::getName, cmd.getChannel()[0]);
	if (chan == getChannels().end())
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *cmd.getChannel().begin())));

	std::vector<Client *>::iterator client_it = chan->isClientInChan(client);
	if (client_it == chan->getClients().end())
		return (sendMessageToClient(client, ERR_NOTONCHANNEL(client, chan->getName())));
	sendMessageToEveryone(RPL_PART(client.getNickname(), client.getUsername(), chan->getName(), cmd.getMsg()), chan->getName());
	client.DecreaseNbChannels();
	chan->getClients().erase(client_it);
	std::vector<Client *>::iterator client_op = client.getOperator(*chan);
	if (client_op != chan->getOperators().end())
		chan->getOperators().erase(client_op);
	if (chan->getClients().size() == 0)
		getChannels().erase(chan);
	return (0);
}