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

void Server::handle_commands(int fd)
{
	Client &client = getClients()[fd];
	Command cmd;
	cmd.parseCmd(client.getMessage());
	if (cmd.getCmd() == "PASS")
		password(client, cmd, this->getPassword());
	else if (cmd.getCmd() == "PING")
		pong(client);
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
	{
		quit(client);
		return ;
	}
	else if (cmd.getCmd() == "TOPIC")
	{
		topic(client,cmd);
	}
	else if (cmd.getCmd() != "CAP LS" || cmd.getCmd() != "WHOIS" || cmd.getCmd() != "WHO")
		sendMessageToClient(getClients()[fd], "Unknown command");
	client.setMessage("");
	client.setNBytes(0);
}

int Server::getPfdIndexByPfd(int pfd)
{
	int i = 0;
	while (i < MAX_FDS - 1)
	{
		if (pfds[i].fd == pfd)
			return i;
		i++;
	}
	return 0;
}

void Server::quit(Client &client)
{
	int tmp_pfd = client.getPfd();
	std::vector<Channel>::iterator chan = getChannels().begin();
	for (; chan != getChannels().end(); chan++)
	{
		std::vector<Client>::iterator chan_client = chan->getClients().begin();
		for (; chan_client != chan->getClients().end(); chan_client++)
		{
			if (chan_client->getNickname() == client.getNickname())
			{
				chan->getClients().erase(chan_client);
				break;
			}
		}
	}
	std::map<int, Client>::iterator client_it = getClients().begin();
	for (; client_it != getClients().end(); client_it++)
	{
		if (client_it->second.getNickname() == client.getNickname())
		{
			int tmp_fd = client_it->first;
			getClients().erase(tmp_fd);
			break;
		}
	}
	close(tmp_pfd);
	deleteClientPfd(getPfdIndexByPfd(tmp_pfd));
	pfd_count--;
}

void Server::pong(Client &client)
{
	sendMessageToClient(client, "PONG localhost");
}

int Server::kick(Client &client, Command &cmd)
{
	std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
	if (!client.is_operator(*chan))
		return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName())));
	for (std::vector<std::string>::iterator users = cmd.getUser().begin(); users != cmd.getUser().end(); users++)
	{
		for (std::vector<Client>::iterator clients = chan->getClients().begin(); clients != chan->getClients().end(); clients++)
		{
			if (*users == clients->getNickname())
			{
				chan->getClients().erase(clients);
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
				return(sendMessageToClient(*clients, RPL_KICK(client.getNickname(), client.getUsername(), chan->getName(), clients->getNickname(), cmd.getMsg())));
			}
		}
	}
	// sendMessageToClient(client, ERR_NOSUCHNICK(client, cmd.getUser()[0]));
	return (0);
}

int Server::sendMessageToEveryone(std::string msg, std::string chan_name)
{
	std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, chan_name);

	if (chan == getChannels().end())
		return (-1);
	for (std::vector<Client>::iterator clients = chan->getClients().begin(); clients != chan->getClients().end(); clients++)
		sendMessageToClient(*clients, msg);
	return (0);
}

int Server::mode(Client &client, Command &cmd)
{
	if (cmd.getChannel().empty() || cmd.getMode().empty())
		return (-1);
	std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
	if (!client.is_operator(*chan))
		return (sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName())));
	std::string arg;
	if (cmd.getUser().empty())
		arg = "";
	else
		arg = cmd.getUser()[0];
	std::string mode = cmd.getMode()[0];
	if (arg == "" && (mode == "+k" || mode == "+o" || mode == "-o" || mode == "+l"))
		return (sendMessageToClient(client, ERR_NEEDMOREPARAMSMODE(client.getNickname(), cmd.getMode()[0])));
	if (cmd.getMode()[0] == "+i" && !chan->getInviteOnly())
		chan->setInviteOnly(1);
	else if (cmd.getMode()[0] == "-i" && chan->getInviteOnly())
		chan->setInviteOnly(0);
	else if (cmd.getMode()[0] == "+o")
	{
		std::vector<std::string>::iterator user = cmd.getUser().begin();
		for (; user != cmd.getUser().end(); user++)
			chan->getOperators().push_back(*user);
	}
	else if (cmd.getMode()[0] == "-o")
	{
		std::vector<std::string>::iterator user = cmd.getUser().begin();
		for (; user != cmd.getUser().end(); user++)
			chan->getOperators().erase(user);
	}
	else if (cmd.getMode()[0] == "+l")
	{
		int limit = atoi(cmd.getUser()[0].c_str());
		if (limit <= 1)
			limit = 1;
		if(limit > 512)
			limit = 512;
		chan->setClientLimit(limit);
	}
	else if (cmd.getMode()[0] == "-l")
		chan->setClientLimit(MAX_CLIENTS);
	else if (cmd.getMode()[0] == "+k")
		chan->setPsswd(cmd.getUser()[0]);
	else if (cmd.getMode()[0] == "-k")
		chan->setPsswd("");
	else if(!mode.empty())
		return(sendMessageToClient(client, ERR_UNKNOWNMODE(client.getNickname(), mode)));
	return (sendMessageToEveryone(RPL_CHANGEMODE(client.getHostname(), chan->getName(), cmd.getMode()[0], arg), chan->getName()));
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
	if(chan.getTopic().empty())
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
		getChannels().back().getOperators().push_back(client.getNickname());
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

int Server::part(Client &client, Command &cmd)
{
	std::vector<Channel>::iterator chan = findValue(getChannels(),
													&Channel::getName, cmd.getChannel()[0]);
	if (chan == getChannels().end())
	{
		return (sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *cmd.getChannel().begin())));
	}
	std::vector<Client>::iterator client_it;
	client_it = findValue(chan->getClients(),
						  &Client::getNickname, client.getNickname());
	if (client_it != chan->getClients().end())
	{
		sendMessageToEveryone(RPL_PART(client_it->getNickname(), client_it->getUsername(), chan->getName(), cmd.getMsg()), chan->getName());
		client_it->DecreaseNbChannels();
		chan->getClients().erase(client_it);
		for (std::vector<std::string>::iterator admin = chan->getOperators().begin(); admin != chan->getOperators().end(); admin++)
		{
			if (client.getNickname() == *admin)
			{
				chan->getOperators().erase(admin);
				break;
			}
		}
		if (chan->getClients().size() == 0)
			getChannels().erase(chan);
	}
	else
		sendMessageToClient(client, ERR_NOTONCHANNEL(client, chan->getName()));
	return (0);
}

void Server::password(Client &client, Command &cmd, std::string server_pass)
{
	(void)cmd;
	if (client.getRegisterProcess() == 0)
	{
		std::string pass = client.getMessage().substr(client.getMessage().find(' ') + 1);
		client.setRegisterProcess(1);
		if (pass == server_pass)
		{
			std::string pass = client.getMessage().substr(client.getMessage().find(' ') + 1);
			if (pass == client.getMessage())
			{
				sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
				return;
			}
			client.setRegisterProcess(1);
			if (pass == server_pass)
			{
				client.setRegisterProcess(1);
			}
			else
			{
				sendMessageToClient(client, ERR_PASSWDMISMATCH);
				client.setRegisterProcess(0);
			}
		}
	}
	else
		sendMessageToClient(client, ERR_ALREADYREGISTRED);
}

int isNickInUse(std::string nick, std::map<int, Client> clients)
{
	std::map<int, Client>::iterator it = clients.begin();
	for (; it != clients.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return 1;
	}
	return 0;
}

void Server::nick(Client &client)
{
	std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
	//TODO - multiple parameter error
	if (isNickInUse(nick, getClients()) == 1)
	{
			sendMessageToClient(client, ERR_NICKNAMEINUSE(nick));
			return;
	}
	if (nick == client.getMessage())
	{
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
		return;
	}
	sendMessageToClient(client, CRPL_NICKCHANGE(client.getNickname(), nick));
	client.setNickname(nick);
	int rp = client.getRegisterProcess();
	client.setRegisterProcess(rp + 1);
	client.setHostname(client.getNickname() + "!" + client.getUsername());
}

void Server::user(Client &client)
{
	if (client.getRegisterProcess() == 2)
	{
		std::string raw_msg = client.getMessage();
		int first_space = raw_msg.find(' ');
		int second_space = raw_msg.find(' ', first_space + 1);
		int third_space = raw_msg.find(' ', second_space + 1);
		if (first_space == (int)std::string::npos || second_space == (int)std::string::npos || third_space == (int)std::string::npos)
		{
			sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
			return;
		}
		std::string username = raw_msg.substr(first_space + 1, second_space - first_space - 1);
		std::string hostname = raw_msg.substr(first_space + 1, third_space - second_space - 1);
		client.setHostname(client.getNickname() + "!" + username);
		client.setUsername(username);
		// std::cout << "USERNAME :" << username << " HOSTNAME :" << hostname << std::endl;
		client.SetIsRegistered(true);
		client.setRegisterProcess(3);
		sendMessageToClient(client, RPL_WELCOME(client).c_str());
	}
	else if (client.getRegisterProcess() > 2)
		sendMessageToClient(client, ERR_ALREADYREGISTRED);
}

void Server::privmsg(Client &client, Command &cmd)
{

	std::string msgval = client.getMessage().substr(client.getMessage().find(':') + 1);
	if (msgval == client.getMessage())
	{
		sendMessageToClient(client, ERR_NOTEXTTOSEND(client));
		return;
	}
	std::vector<std::string> chan_names = cmd.getChannel();
	std::vector<std::string>::iterator it_chname = chan_names.begin();
	if (it_chname == chan_names.end())
	{
		int first_space = client.getMessage().find(' ');
		int second_space = client.getMessage().find(' ', first_space + 1);
		if (second_space == (int)std::string::npos)
		{
			sendMessageToClient(client, ERR_NORECIPIENT(client));
			return;
		}
		std::string target_client_nickname = client.getMessage().substr(first_space + 1, second_space - first_space - 1);
		Client *target_client = getOneClientByNickname(target_client_nickname);
		if (!target_client)
		{
			sendMessageToClient(client, ERR_NOSUCHNICK(client, target_client_nickname));
			return;
		}
		sendMessageToClient(*target_client, RPL_PRIVMSG(client.getNickname(), target_client->getNickname(), msgval));
	}
	else
	{
		while (it_chname != chan_names.end())
		{
			std::vector<Channel>::iterator it_ch = findValue(getChannels(),
															 &Channel::getName, *it_chname);
			if (it_ch == getChannels().end())
			{
				sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *it_chname));
				return;
			}
			std::vector<Client>::iterator it_cli = it_ch->getClients().begin();
			for (; it_cli != it_ch->getClients().end(); it_cli++)
			{
				if (getRealNickname(it_cli->getNickname()) != client.getNickname())
					sendMessageToClient(*it_cli, CMSG_PRIVMSG_CH(client, *it_chname, msgval));
			}
			it_chname++;
		}
	}
}

bool isClientInChannel(Client &client, Channel &ch)
{
	std::vector<Client>::iterator client_it = ch.getClients().begin();
	for (;client_it != ch.getClients().end();client_it++)
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
	{
		sendMessageToClient(*client_it, msg);
	}
}

bool isClientOp(Client &client, Channel &channel)
{
	std::vector<std::string>::iterator chan_it = channel.getOperators().begin();
	for (; chan_it != channel.getOperators().end() ; chan_it++)
	{
		if (client.getNickname() == *chan_it)
			return true;
	}
	return false;
}



int Server::topic(Client &client, Command &cmd)
{
	std::string raw_msg = client.getMessage();
	std::string topic_str = raw_msg.substr(raw_msg.find(':') + 1);
	std::vector<std::string>::iterator ch_names = cmd.getChannel().begin();
	if (ch_names == cmd.getChannel().end())
		return(sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "TOPIC")));
	std::vector<Channel>::iterator it_ch = findValue(getChannels(),
													&Channel::getName, *ch_names);
	if (it_ch == getChannels().end())
		return(sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *ch_names)));
	if (!isClientInChannel(client, *it_ch))
		return(sendMessageToClient(client, ERR_NOTONCHANNEL(client, *ch_names)));
	if (topic_str == raw_msg)
		return(sendMessageToClient(client, RPL_TOPIC(client.getNickname(), *ch_names,it_ch->getTopic())));
	else if (it_ch->getTopicOp() && !isClientOp(client, *it_ch))
		return(sendMessageToClient(client, ERR_CHANOPRIVSNEEDED(client, *ch_names)));
	else
	{
		it_ch->setTopic(topic_str);
		sendMessageToEveryClientInChannel(TOPIC(client, cmd.getChannel()[0], topic_str), *it_ch);
	}
	// std::cout << it_ch->getTopicOp() << " " << isClientOp(client, *it_ch) << std::endl;
	return (0);
}

