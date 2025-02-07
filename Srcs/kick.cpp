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
	std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
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
	for (std::vector<std::string>::iterator users = cmd.getUser().begin(); users != cmd.getUser().end(); users++)
	{
		for (std::vector<Client>::iterator clients = chan->getClients().begin(); clients != chan->getClients().end(); clients++)
		{
			if (*users == clients->getNickname())
			{
				if (clients->is_operator(*chan))
				{
					std::vector<std::string>::iterator admin = chan->getOperators().begin();
					for (; admin != chan->getOperators().end(); admin++)
					{
						if (*admin == clients->getNickname())
							chan->getOperators().erase(admin);
					}
				}
				sendMessageToEveryone(RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), clients->getNickname(), cmd.getMsg()), chan->getName());
				sendMessageToClient(*clients, RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), clients->getNickname(), cmd.getMsg()));
				chan->getClients().erase(clients);
                return(0);
			}
		}
	}
	return (sendMessageToClient(client, ERR_USERNOTINCHAN2(client.getNickname(), cmd.getUser()[0], cmd.getChannel()[0])));
}

