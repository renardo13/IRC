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


int Server::topic(Client &client, Command &cmd)
{
	std::string raw_msg = client.getMessage();
	std::string topic_str = cmd.getMsg();
	std::vector<std::string>::iterator ch_names = cmd.getChannel().begin();
	if (ch_names == cmd.getChannel().end())
		return (sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd())));
	std::vector<Channel>::iterator it_ch = findValue(getChannels(),
													 &Channel::getName, *ch_names);

	if (it_ch == getChannels().end())
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *ch_names)));
	if (it_ch->isClientInChan(client) != it_ch->getClients().end())
	{
		if (topic_str.empty())
			return ((it_ch->getTopic() == "") ? sendMessageToClient(client, RPL_NOTOPIC(client, *ch_names)) : sendMessageToClient(client, RPL_TOPIC(client.getNickname(), *ch_names, it_ch->getTopic())));
		if (it_ch->isClientInChan(client) == it_ch->getClients().end())
			return (sendMessageToClient(client, ERR_NOTONCHANNEL(client, *ch_names)));
		else if (it_ch->getTopicOp() && client.getOperator(*it_ch) == it_ch->getOperators().end())
			return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), *ch_names)));
		else
		{
			it_ch->setTopic(topic_str);
			sendMessageToEveryClientInChannel(TOPIC(client, cmd.getChannel()[0], topic_str), *it_ch);
		}
	}
	else
		return ((it_ch->getTopic() == "") ? sendMessageToClient(client, RPL_NOTOPIC(client, *ch_names)) : sendMessageToClient(client, RPL_TOPIC(client.getNickname(), *ch_names, it_ch->getTopic())));
	return (0);
}
