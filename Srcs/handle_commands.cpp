#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include <string>
#include <vector>


// Generic function to find a value of a certain type in a certain container type. ("::Value_type" is the type of elements that the container store)
template <typename Container, typename AttributeType,
	typename Value> typename Container::iterator findValue(Container &container,
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
	std::string msg = client.getMessage();
	std::string cmd = msg.substr(0, msg.find(' '));
	std::cout << "MSG: " << msg << std::endl;
	// TO-DO: without registeration no message should be handled.
	if (cmd == "PASS")
	{
		if (client.getRegisterProcess() == 0)
		{
			std::string passwd = msg.substr(msg.find(' ') + 1, msg.size() - 2);
			std::cout << "pass is: " << this->pass << std::endl;
			if (passwd == this->pass)
			{
				client.setRegisterProcess(1);
				std::cout << "PASS IS CORRECT" << std::endl;
			}
		}
		else
			sendMessageToClient(client, ERR_ALREADYREGISTRED);
	}
	else if (cmd == "JOIN")
	{
		join(getClients()[fd]);
	}
	else if (cmd == "PART")
	{
		part(getClients()[fd]);
	}
    else if (cmd == "KICK")
	{
		kick(getClients()[fd]);
	}
	else if (cmd == "NICK")
	{
		if (client.getRegisterProcess() == 1)
		{
			std::string nick = msg.substr(msg.find(' ') + 1);
			client.setNickname(nick);
			client.setRegisterProcess(2);
		}
		else
		{
			std::string nick = msg.substr(msg.find(' ') + 1);
			sendMessageToClient(client, CRPL_NICKCHANGE(client.getNickname(), nick));
			client.setNickname(nick);
		}
	}
	else if (cmd == "USER")
	{
		if (client.getRegisterProcess() == 2)
		{
			std::string user = msg.substr(msg.find(' ') + 1);
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

void Server::kick(Client &client)
{
    Command cmd;
	cmd.parseCmd(client.getMessage());

    

    // if(client.getNickname()[0] != '@')
    // {
    //     std::cout << "Client is not operator and cannot kick " << " from " << chan << std::endl;
    // }


}

void Server::join(Client &client)
{
	std::string mess = client.getMessage().substr(client.getMessage().find(' '));
	std::string name = mess.substr(mess.find_first_not_of(' '));
	if (name.empty() || name[0] != '#')
	{
		std::cout << "Channel name is invalid\n";
		return ;
	}
	name = name.substr(1);
	std::vector<Channel>::iterator chan = findValue(getChannels(),
			&Channel::getName, name);
	if (chan != getChannels().end())
	{
		chan->getClients().push_back(client);
		std::cout << "Client reached successfully an existing channel\n";
	}
	else
	{
		Channel channel(name);
		this->setChannel(channel);
        client.setNickname('@' + client.getNickname());
		getChannels().back().getClients().push_back(client);
		std::cout << "Channel was successfully created\n";
	}
}
void Server::part(Client &client)
{
	std::string mess = client.getMessage().substr(client.getMessage().find(' '));
	std::string chan_name = mess.substr(mess.find_first_not_of(' '));

	std::vector<Channel>::iterator chan = findValue(getChannels(),
			&Channel::getName, chan_name);
	if (chan == getChannels().end())
	{
		std::cout << "Channel does not exist in the server\n";
		return ;
	}
	std::vector<Client>::iterator client_it = findValue(chan->getClients(),
			&Client::getNickname, client.getNickname());
	if (client_it == chan->getClients().end())
	{
		std::cout << "Client is not in this channel\n";
	}
	else
	{
		std::cout << "Client left the channel\n";
		chan->getClients().erase(client_it);
		if (chan->getClients().size() == 0)
		{
			getChannels().erase(chan);
			std::cout << "Channel was suppressed\n";
		}
	}
	print();
}

void Server::quit(Client &client)
{
	
}


