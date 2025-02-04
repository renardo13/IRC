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
	// std::cout << "MSG: " << client.getMessage() << std::endl;
	if (cmd.getCmd() == "PASS")
	{
		password(client, cmd, this->getPassword());
	}
	else if (cmd.getCmd() == "PING")
	{
		pong(getClients()[fd], cmd);
	}
	else if (cmd.getCmd() == "JOIN")
	{
		join(getClients()[fd], cmd);
	}
	else if (cmd.getCmd() == "PART")
	{
		part(getClients()[fd], cmd);
	}
	else if (cmd.getCmd() == "KICK")
	{
		kick(getClients()[fd], cmd);
	}
	else if (cmd.getCmd() == "NICK")
	{
		nick(client, cmd);
	}
	else if (cmd.getCmd() == "USER")
	{
		user(client, cmd);
	}
	else if (cmd.getCmd() == "PRIVMSG")
	{
		privmsg(client, cmd);
	}
	else if (cmd.getCmd() == "MODE")
	{
		mode(client, cmd);
	}
	else if (cmd.getCmd() == "QUIT")
	{
		quit(client);
		return ;
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
	for(; chan != getChannels().end(); chan++)
	{
		std::vector<Client>::iterator chan_client = chan->getClients().begin();
		for(; chan_client != chan->getClients().end(); chan_client++)
		{
			if(chan_client->getNickname() == client.getNickname())
			{
				chan->getClients().erase(chan_client);
				break;
			}
		}
	}
	std::map<int, Client>::iterator client_it = getClients().begin();
	for(; client_it != getClients().end(); client_it++)
	{
		if(client_it->second.getNickname() == client.getNickname())
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

void Server::pong(Client &client, Command &cmd)
{
	sendMessageToClient(client, "PONG " + cmd.getCmd());
}

void Server::kick(Client &client, Command &cmd)
{
	(void)client;
	(void)cmd;
	// if(client.getNickname()[0] != '@')
	// {
	//     std::cout << "Client is not operator and cannot kick " << " from " << chan << std::endl;
	// }
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

void Server::mode(Client &client, Command &cmd)
{
	if (cmd.getChannel().empty() || cmd.getMode().empty())
		return;
	std::vector<Channel>::iterator chan = findValue(getChannels(), &Channel::getName, cmd.getChannel()[0]);
	if (chan != getChannels().end())
	{
		std::vector<std::string>::iterator find_op = chan->getOperators().begin();
		for (; find_op != chan->getOperators().end(); find_op++)
		{
			if (*find_op == client.getNickname())
			{
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
				else if (cmd.getMode()[0] == "+t")
					chan->setClientLimit(atoi(cmd.getUser()[0].c_str()));
				else if (cmd.getMode()[0] == "-t")
					chan->setClientLimit(0);
				else if (cmd.getMode()[0] == "+k")
				{
					chan->setPsswd(cmd.getUser()[0]);
				}
				else if (cmd.getMode()[0] == "-k")
				{
					chan->setPsswd(NULL);
				}
				break;
			}
		}
		if (find_op == chan->getOperators().end())
			sendMessageToClient(client, ERR_NOTOPERATOR(client.getNickname(), chan->getName()));
	}
	print();
	// std::vector<std::string> client_op = findValue(getClients(), &Client::getNickname, client.getNickname());
}

int Server::reach_channel(Client &client, Command &cmd, Channel &chan, std::string chan_name)
{
	if (chan.getClients().size() > MAX_CLIENTS)
		return (sendMessageToClient(client, ERR_TOOMANYCLIENTS(client.getNickname(), chan.getName())));
	else if (chan.getClientLimit() != 0 && ((int)chan.getClients().size() > chan.getClientLimit()))
		return (sendMessageToClient(client, ERR_TOOMANYCLIENTS(client.getNickname(), chan.getName())));
	else if (client.getNbChannels() > 10)
		return (sendMessageToClient(client, ERR_TOOMANYCHANNELS(client.getNickname(), chan.getName())));
	else if (chan.getInviteOnly())
		return (sendMessageToClient(client, INVITE_ONLY(client.getNickname(), chan.getName())));
	else if (!chan.getPsswd().empty())
	{
		if (!cmd.getUser().empty() && cmd.getUser()[0] != chan.getPsswd())
			return (sendMessageToClient(client, ERR_PSSWD(client.getNickname(), chan.getName())));
	}
	client.IncreaseNbChannels();
	chan.getClients().push_back(client);
	sendMessageToEveryone(RPL_JOIN(client.getHostname(), chan_name), chan_name);
	sendMessageToClient(client, RPL_JOIN(client.getHostname(), chan_name) + \
								RPL_NAMES(client.getNickname(), chan_name, getClientsList(chan)) + \
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
	// std::cout << "============= Avant part ==============\n";
	// print();
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
				chan->getOperators().erase(admin);
		}
		if (chan->getClients().size() == 0)
		{
			getChannels().erase(chan);
		}
	}
	else
		sendMessageToClient(client, ERR_NOTONCHANNEL(client, chan->getName()));
	// std::cout << "============= Apres part ==============\n";
	// print();
	return (0);
}

void Server::password(Client &client, Command cmd, std::string server_pass)
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
				// std::cout << "PASS IS CORRECT" << std::endl;
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
	for(; it != clients.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return 1;
	}
	return 0;
}

void Server::nick(Client &client, Command cmd)
{
	(void)cmd;
	std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
	if (isNickInUse(nick, getClients()) == 1)
		{
			sendMessageToClient(client, ERR_NICKNAMEINUSE(nick));
			client.setRegisterProcess(2);
			client.setNickname("ft_irc_enjoyer");
			return;
		}
	if (client.getRegisterProcess() == 1)
	{
		if (nick == client.getMessage())
		{
			sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
			return;
		}
		client.setNickname(nick);
		client.setRegisterProcess(2);
	}
	else if (client.getRegisterProcess() != 0)
	{
		sendMessageToClient(client, CRPL_NICKCHANGE(client.getNickname(), nick));
		client.setNickname(nick);
		client.setHostname(client.getNickname() + "!" + client.getUsername());
	}
}



void Server::user(Client &client, Command cmd)
{
	(void)cmd;
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
		std::string username =  raw_msg.substr(first_space + 1, second_space - first_space - 1);
		std::string hostname =  raw_msg.substr(first_space + 1, third_space - second_space - 1);
		client.setHostname(client.getNickname() + "!" + username);
		client.setUsername(username);
		// std::cout << "USERNAME :" << username << " HOSTNAME :" << hostname << std::endl;
		client.SetIsRegistered(true);
		client.setRegisterProcess(3);
		sendMessageToClient(client, RPL_WELCOME(client).c_str());
	}
	else if (client.getRegisterProcess() != 2)
		sendMessageToClient(client, ERR_ALREADYREGISTRED);
}

void Server::privmsg(Client &client, Command cmd)
{
	
	std::string msgval = client.getMessage().substr(client.getMessage().find(':') + 1);
	if (msgval == client.getMessage())
	{
		sendMessageToClient(client, ERR_NOTEXTTOSEND(client));
		return ;
	}
	std::vector<std::string> chan_names = cmd.getChannel();
	std::vector <std::string>::iterator it_chname = chan_names.begin();
	if (it_chname == chan_names.end())
	{
		int first_space = client.getMessage().find(' ');
		int second_space = client.getMessage().find(' ', first_space + 1);
		if (second_space == (int)std::string::npos)
		{
			sendMessageToClient(client, ERR_NORECIPIENT(client));
			return ;
		}
		std::string target_client_nickname = client.getMessage().substr(first_space + 1, second_space - first_space - 1);
		Client* target_client = getOneClientByNickname(target_client_nickname);
		if (!target_client)
		{
			sendMessageToClient(client, ERR_NOSUCHNICK(client, target_client_nickname));
			return;
		}
		std::cout << "TARGET: " << target_client->getPfd() << std::endl;
		if (sendMessageToClient(*target_client, CMSG_PRIVMSG_CL(client, target_client_nickname, msgval)) == -1)
			std::cout << "Message is not sent" <<std::endl;
		else
			std::cout << "Message is sent succesfully" << std::endl;
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
				return ;
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

