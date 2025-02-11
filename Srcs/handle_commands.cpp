#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"
#include <string>
#include <vector>
#include <stdio.h>

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

int Server::handle_commands(int fd)
{
	Client &client = getClients()[fd];
	Command cmd;
	cmd.parseCmd(client.getMessage());
	if (cmd.getCmd() == "PASS")
	{
		password(client, getPassword());
		return 1;
	}
	else if (cmd.getCmd() == "PING")
		pong(client, cmd);
	else if (cmd.getCmd() == "JOIN")
		join(client, cmd);
	else if (cmd.getCmd() == "PART")
		part(client, cmd);
	else if (cmd.getCmd() == "KICK")
		kick(client, cmd);
	else if (cmd.getCmd() == "NICK")
	{
		nick(client);
		return 1;
	}
	else if (cmd.getCmd() == "USER")
		user(client);
	else if (cmd.getCmd() == "PRIVMSG")
		privmsg(client, cmd);
	else if (cmd.getCmd() == "MODE")
		mode(client, cmd);
	else if (cmd.getCmd() == "QUIT")
		return (quit(client));
	else if (cmd.getCmd() == "TOPIC")
		topic(client, cmd);
	else if (cmd.getCmd() == "INVITE")
		invite(client, cmd);
	else if (cmd.getCmd() != "CAP" && cmd.getCmd() != "WHOIS" && cmd.getCmd() != "WHO")
		sendMessageToClient(client, ERR_UNKNOWNCOMMAND(client.getNickname(), cmd.getCmd()));
	client.setMessage("");
	client.setNBytes(0);
	return (0);
}

void Server::pong(Client &client, Command &cmd)
{
	sendMessageToClient(client, "PONG " + cmd.getUser()[0]);
}

int Server::part(Client &client, Command &cmd)
{
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

void Server::sendMessageToEveryClientInChannel(std::string msg, Channel &channel)
{
	std::vector<Client *>::iterator client_it = channel.getClients().begin();
	for (; client_it != channel.getClients().end(); client_it++)
		sendMessageToClient(*(*client_it), msg);
}

int Server::topic(Client &client, Command &cmd)
{
	std::string raw_msg = client.getMessage();
	std::string topic_str = cmd.getMsg();
	std::vector<std::string>::iterator ch_names = cmd.getChannel().begin();
	if (ch_names == cmd.getChannel().end())
		return (sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC")));
	std::vector<Channel>::iterator it_ch = findValue(getChannels(),
													 &Channel::getName, *ch_names);

	if (it_ch == getChannels().end())
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *ch_names)));
	if (it_ch->isClientInChan(client) != it_ch->getClients().end())
	{
		if (topic_str == raw_msg)
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
		sendMessageToClient(client, RPL_TOPIC(client.getNickname(), it_ch->getName(), it_ch->getTopic()));
	return (0);
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
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
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
