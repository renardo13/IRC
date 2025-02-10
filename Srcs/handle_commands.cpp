#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"
#include <string>
#include <vector>
#include <stdio.h>

int Server::handle_commands(int fd)
{
	Client &client = getClients()[fd];
    std::cout << "Handle commad: Adresse de :" << client.getNickname() << " ";
    printf("%p\n", &client);

	Command cmd;
	cmd.parseCmd(client.getMessage());
	if (cmd.getCmd() == "PASS")
		password(client, getPassword());
	else if (cmd.getCmd() == "PING")
		pong(client, cmd);
	else if (cmd.getCmd() == "JOIN")
		join(client, cmd);
	else if (cmd.getCmd() == "PART")
		part(client, cmd);
	else if (cmd.getCmd() == "KICK")
		kick(client, cmd);
	else if (cmd.getCmd() == "NICK")
		nick(client);
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
	else if (cmd.getCmd() != "CAP" || cmd.getCmd() != "WHOIS" || cmd.getCmd() != "WHO")
		sendMessageToClient(client, ERR_UNKNOWNCOMMAND(client.getNickname(), cmd.getCmd()));
	client.setMessage("");
	client.setNBytes(0);
	print();
	return (0);
}

void Server::pong(Client &client, Command &cmd)
{
	sendMessageToClient(client, "PONG " + cmd.getUser()[0]);
}

int Server::part(Client &client, Command &cmd)
{
	Channel* chan = findValue(getChannels(),
													&Channel::getName, cmd.getChannel()[0]);
	if (chan != NULL)
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *cmd.getChannel().begin())));
	Client* client_it;
	client_it = findValue(chan->getClients(),
						  &Client::getNickname, client.getNickname());
	if (client_it == NULL)
		return (sendMessageToClient(client, ERR_NOTONCHANNEL(client, chan->getName())));
	sendMessageToEveryone(RPL_PART(client_it->getNickname(), client_it->getUsername(), chan->getName(), cmd.getMsg()), chan->getName());
	client_it->DecreaseNbChannels();
	chan->getClients().erase(std::find(chan->getClients().begin(), chan->getClients().end(), client));
	if (client.is_operator(*chan))
		chan->getOperators().erase(std::find(chan->getOperators().begin(), chan->getOperators().end(), client.getNickname()));
	if (chan->getClients().size() == 0)
		getChannels().erase(std::find(getChannels().begin(), getChannels().end(), chan->getName()));
	return (0);
}

bool isClientInChannel(Client &client, Channel &ch)
{
	std::vector<Client>::iterator client_it = ch.getClients().begin();
	for (; client_it != ch.getClients().end(); client_it++)
	{
		if (getRealNickname(client_it->getNickname()) == client.getNickname())
			return true;
	}
	return false;
}

void Server::sendMessageToEveryClientInChannel(std::string msg, Channel &channel)
{
	std::vector<Client>::iterator client_it = channel.getClients().begin();
	for (; client_it != channel.getClients().end(); client_it++)
		sendMessageToClient(*client_it, msg);
}

int Server::topic(Client &client, Command &cmd)
{
	std::string raw_msg = client.getMessage();
	std::string topic_str = cmd.getMsg();
	std::vector<std::string>::iterator ch_names = cmd.getChannel().begin();
	if (ch_names == cmd.getChannel().end())
		return (sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC")));
	Channel* it_ch = findValue(getChannels(),
													 &Channel::getName, *ch_names);
	if (it_ch != NULL)
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *ch_names)));
	if (topic_str == raw_msg)
		return ((it_ch->getTopic() == "") ? sendMessageToClient(client, RPL_NOTOPIC(client, *ch_names)) : sendMessageToClient(client, RPL_TOPIC(client.getNickname(), *ch_names, it_ch->getTopic())));
	if (!isClientInChannel(client, *it_ch))
		return (sendMessageToClient(client, ERR_NOTONCHANNEL(client, *ch_names)));
	else if (it_ch->getTopicOp() && !client.is_operator(*it_ch))
		return (sendMessageToClient(client, ERR_CHANOPRIVSNEEDED(client, *ch_names)));
	else
	{
		it_ch->setTopic(topic_str);
		sendMessageToEveryClientInChannel(TOPIC(client, cmd.getChannel()[0], topic_str), *it_ch);
	}
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
	Channel* it_ch = findValue(getChannels(),
													 &Channel::getName, *ch_names);
	if (it_ch != NULL)
	{
		sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *ch_names));
		return;
	}
	if (!isClientInChannel(client, *it_ch))
	{
		sendMessageToClient(client, ERR_NOTONCHANNEL(client, it_ch->getName()));
		return;
	}
	if (isClientInChannel(*target_client, *it_ch))
	{
		sendMessageToClient(client, ERR_USERONCHANNEL(client, target_client_nickname, *ch_names));
		return;
	}
	sendMessageToClient(client, RPL_INVITING(client, target_client_nickname, *ch_names));
	sendMessageToClient(*target_client, INVITE(client, target_client_nickname));
}
