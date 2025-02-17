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

int Server::privmsg(Client &client, Command &cmd)
{
	if(cmd.getChannel().empty())
		return(sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd())));
	std::vector<Channel>::iterator chan = findValue(getChannels(),
													&Channel::getName, cmd.getChannel()[0]);
	if (chan == getChannels().end())
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *cmd.getChannel().begin())));
	std::vector<Client *>::iterator client_it = chan->isClientInChan(client);
	if (client_it == chan->getClients().end())
		return (sendMessageToClient(client, ERR_NOTONCHANNEL(client, chan->getName())));
	std::string msgval = client.getMessage().substr(client.getMessage().find(':') + 1);
	if (msgval == client.getMessage())
		return(sendMessageToClient(client, ERR_NOTEXTTOSEND(client)));
	std::vector<std::string> chan_names = cmd.getChannel();
	std::vector<std::string>::iterator it_chname = chan_names.begin();
	if (it_chname == chan_names.end())
	{
		int first_space = client.getMessage().find(' ');
		int second_space = client.getMessage().find(' ', first_space + 1);
		if (second_space == (int)std::string::npos)
			return(sendMessageToClient(client, ERR_NORECIPIENT(client)));
		std::string target_client_nickname = client.getMessage().substr(first_space + 1, second_space - first_space - 1);
		Client *target_client = getOneClientByNickname(target_client_nickname);
		if (!target_client)
			return(sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), target_client_nickname)));
		sendMessageToClient(*target_client, RPL_PRIVMSG(client.getNickname(), target_client->getNickname(), msgval));
	}
	else
	{
		while (it_chname != chan_names.end())
		{
			std::vector<Channel>::iterator it_ch = findValue(getChannels(),
															 &Channel::getName, *it_chname);
			if (it_ch == getChannels().end())
				return(sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *it_chname)));
			std::vector<Client*>::iterator it_cli = it_ch->getClients().begin();
			for (; it_cli != it_ch->getClients().end(); it_cli++)
			{
				if (getRealNickname((*it_cli)->getNickname()) != client.getNickname())
					sendMessageToClient(*(*it_cli), CMSG_PRIVMSG_CH(client, *it_chname, msgval));
			}
			it_chname++;
		}
	}
	return(0);
}
