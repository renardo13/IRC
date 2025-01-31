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
		if (((*it).*getter)() == value)
			return (it);
	return (container.end());
}

void Server::handle_commands(int fd)
{
	Client &client = getClients()[fd];
	Command cmd;
	cmd.parseCmd(client.getMessage());

	if (cmd.getCmd() == "PASS")
	{
		if (client.getRegisterProcess() == 0)
		{
			std::string pass = client.getMessage().substr(client.getMessage().find(' ') + 1);
			client.setRegisterProcess(1);
			if (pass == this->pass)
			{
				client.setRegisterProcess(1);
				std::cout << "PASS IS CORRECT" << std::endl;
			}
		}
		else
			sendMessageToClient(client, ERR_ALREADYREGISTRED);
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
		if (client.getRegisterProcess() == 1)
		{
			std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
			client.setNickname(nick);
			client.setRegisterProcess(2);
		}
		else
		{
			std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
			sendMessageToClient(client, CRPL_NICKCHANGE(client.getNickname(), nick));
			client.setNickname(nick);
		}
	}
	else if (cmd.getCmd() == "USER")
	{
		if (client.getRegisterProcess() == 2)
		{
			std::string user = cmd.getMsg().substr(cmd.getMsg().find(' ') + 1);
			client.setUsername(user);
			client.SetIsRegistered(true);
			client.setRegisterProcess(3);
			sendMessageToClient(client, RPL_WELCOME(client).c_str());
			std::cout << "Welcome message shouldve been sent" << std::endl;
		}
		else
			client.setRegisterProcess(0);
	}
	else
		sendMessageToClient(getClients()[fd], "Unknown command");
	client.setMessage("");
	client.setNBytes(0);
}

void Server::kick(Client &client, Command& cmd)
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

	if(chan == getChannels().end())
		return(-1);
	for(std::vector<Client>::iterator clients = chan->getClients().begin(); clients != chan->getClients().end(); clients++)
		sendMessageToClient(*clients, msg);
	return(0);
}

void Server::join(Client &client, Command& cmd)
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
			chan_find->getClients().push_back(client);
			client.setHostname("MyhostName");
			break;
		}
		else
		{
			Channel channel(*chan_name);
			this->setChannel(channel);
			client.setHostname("MyhostName");
			getChannels().back().getClients().push_back(client);
			getChannels().back().getClients().back().setNickname('@' + client.getNickname());
			break;
		}
	}
	sendMessageToEveryone(RPL_JOIN(client, *chan_name), *chan_name);
	print();
}

void Server::part(Client &client, Command& cmd)
{
	std::vector<Channel>::iterator chan = findValue(getChannels(),
													&Channel::getName, *cmd.getChannel().begin());
	if (chan == getChannels().end())
	{
		sendMessageToClient(client, ERR_NOSUCHCHANNEL(client.getNickname(), *cmd.getChannel().begin()));
		return;
	}
	chan->getClients().erase(findValue(chan->getClients(),
													&Client::getNickname, client.getNickname()));
	sendMessageToClient(client, RPL_PART(client, *cmd.getChannel().begin()));
	if (chan->getClients().size() == 0)
		getChannels().erase(chan);
	print();
}
