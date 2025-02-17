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

void Server::invite(Client &client, Command &cmd)
{
	std::string raw_msg = client.getMessage();
	int first_space = client.getMessage().find(' ');
	int second_space = raw_msg.find(' ', first_space + 1);
	std::vector<std::string>::iterator ch_names = cmd.getChannel().begin();
	if (second_space == (int)std::string::npos)
	{
		sendMessageToClient(client, ERR_NORECIPIENT(client));
		return;
	}
	if (cmd.getChannel().begin() == cmd.getChannel().end())
	{
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd()));
		return;
	}
	std::string target_client_nickname = client.getMessage().substr(first_space + 1, second_space - first_space - 1);
	Client *target_client = getOneClientByNickname(target_client_nickname);
	if (!target_client)
	{
		sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), target_client_nickname));
		return;
	}
	std::vector<Channel>::iterator it_ch = findValue(getChannels(),
													 &Channel::getName, *ch_names);
	if (it_ch == getChannels().end())
	{
		sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *ch_names));
		return;
	}
	if (it_ch->isClientInChan(client) == it_ch->getClients().end())
	{
		sendMessageToClient(client, ERR_NOTONCHANNEL(client, it_ch->getName()));
		return;
	}
	if (it_ch->isClientInChan(target_client_nickname) != it_ch->getClients().end()) //WARNING
	{
		sendMessageToClient(client, ERR_USERONCHANNEL(client, target_client_nickname, *ch_names));
		return;
	}
	sendMessageToClient(client, RPL_INVITING(client, target_client_nickname, *ch_names));
	sendMessageToClient(*target_client, INVITE(client, target_client_nickname, *ch_names));
	it_ch->addClientToInviteList(target_client_nickname);
}
