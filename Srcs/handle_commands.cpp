#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"
#include <string>
#include <vector>

// Generic function to find a value of a certain type in a certain container type. ("::Value_type" is the type of elements that the container store)
template <typename Container, typename AttributeType,
		  typename Value>
typename Container::iterator findValue(Container &container,
									   AttributeType (Container::value_type::*getter)() const, const Value value)
{
	typename Container::iterator it = container.begin();
	for (; it != container.end(); it++)
	{
		// std::cout << "((*it).*getter)() " << ((*it).*getter)() << " | Value : " << value << std::endl;
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
	std::cout << "MSG: " << client.getMessage() << std::endl;
	if (cmd.getCmd() == "PASS")
	{
		password(client, cmd, this->getPassword());
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
	else if (cmd.getCmd() == "PONG")
	{
		privmsg(client, cmd);
	}
	else
		sendMessageToClient(getClients()[fd], "Unknown command");
	client.setMessage("");
	client.setNBytes(0);
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

// void Server::mode(Client &client, Command &cmd)
// {
// 	if(client.get)
// }

void Server::join(Client &client, Command &cmd)
{
	if (cmd.getChannel().empty())
	{
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), cmd.getCmd()));
		return;
	}
	std::vector<std::string>::iterator chan_name = cmd.getChannel().begin();
	for (; chan_name != cmd.getChannel().end(); chan_name++)
	{
		std::vector<Channel>::iterator chan_find = findValue(getChannels(), &Channel::getName, *chan_name);
		if (chan_find != getChannels().end())
		{
			if (client.getNbChannels() > 10)
				sendMessageToClient(client, ERR_TOOMANYCHANNELS(client.getNickname(), chan_find->getName()));
			else
			{
				chan_find->getClients().push_back(client);
				client.setHostname("MyhostName");
				sendMessageToEveryone(RPL_JOIN(client, *chan_name), *chan_name);
				client.setNbChannels(1);
			}
		}
		else
		{
			if (client.getNbChannels() > 10)
				sendMessageToClient(client, ERR_TOOMANYCHANNELS(client.getNickname(), chan_find->getName()));
			else
			{
				Channel channel(*chan_name);
				channel.getOperators().push_back(client.getNickname());
				this->setChannel(channel);
				client.setHostname("MyhostName");
				getChannels().back().getClients().push_back(client);
				getChannels().back().getOperators().push_back(client.getNickname());
				sendMessageToEveryone(RPL_JOIN(client, *chan_name), *chan_name);
				client.setNbChannels(1);
			}
		}
	}
	print();
}


void Server::part(Client &client, Command &cmd)
{
	std::vector<Channel>::iterator chan = findValue(getChannels(),
													&Channel::getName, cmd.getChannel()[0]);
	if (chan == getChannels().end())
	{
		sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *cmd.getChannel().begin()));
		return;
	}
	std::vector<Client>::iterator client_it;
	if (client.is_operator(chan->getOperators()))
	{
		std::cout << "Client is operator !\n";
	}
	else
		client_it = findValue(chan->getClients(),
							  &Client::getNickname, client.getNickname());
	if (client_it != chan->getClients().end())
	{
		chan->getClients().erase(client_it);
		client.setNbChannels(-1);
		sendMessageToEveryone(RPL_PART(client, chan->getName()), chan->getName());
		if (chan->getClients().size() == 0)
			getChannels().erase(chan);
		std::cout << "Remove client from channel\n";
	}
	else
		sendMessageToClient(client, ERR_NOTONCHANNEL(client, chan->getName()));
	print();
}

void Server::password(Client &client, Command cmd, std::string server_pass)
{
	(void)cmd;
	if (client.getRegisterProcess() == 0)
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
				std::cout << "PASS IS CORRECT" << std::endl;
			}
			else
			{
				sendMessageToClient(client, ERR_PASSWDMISMATCH);
				client.setRegisterProcess(0);
			}
		}
		else
			sendMessageToClient(client, ERR_ALREADYREGISTRED);
}

void Server::nick(Client &client, Command cmd)
{
	(void)cmd;
	if (client.getRegisterProcess() == 1)
	{
		std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
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
		std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
		sendMessageToClient(client, CRPL_NICKCHANGE(client.getNickname(), nick));
		client.setNickname(nick);
	}
}



void Server::user(Client &client, Command cmd)
{
	//USER <username> <hostname> <servername> :<realname>
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
		client.setHostname(hostname);
		client.setUsername(username);
		std::cout << "USERNAME :" << username << " HOSTNAME :" << hostname << std::endl;
		client.SetIsRegistered(true);
		client.setRegisterProcess(3);
		sendMessageToClient(client, RPL_WELCOME(client).c_str());
	}
	else if (client.getRegisterProcess() != 0)
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
					sendMessageToClient(*it_cli, CMSG_PRIVMSG_CH(client,*it_chname,msgval));
			}
			it_chname++;
		}
	}
}

// void Server::quit(Client &client)
// {
// 	std::string raw_msg = client.getMessage();
// 	std::string msg = raw_msg.substr(raw_msg.find(':'));
// 	if (msg == raw_msg)
// 		msg = "";
	
// 	//TO-DO
// 	// Delete from pfds, clients, and channels(?)


// }
