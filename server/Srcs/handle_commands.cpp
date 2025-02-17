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
	std::cout << std::endl << BOLD << GREEN << "[CLIENT] => " << client.getMessage() << RESET << std::endl;
	if (cmd.getCmd() == "PASS")
	{
		password(client, getPassword());
		return 1;
	}
	else if (client.getIsPasswordCorrect())
	{
		if (cmd.getCmd() == "PING")
			pong(client, cmd);
		else if (cmd.getCmd() == "JOIN" && client.getNickname() != "")
			join(client, cmd);
		else if (cmd.getCmd() == "PART")
			part(client, cmd);
		else if (cmd.getCmd() == "KICK")
			kick(client, cmd);
		else if (cmd.getCmd() == "NICK")
			return(nick(client), 1);
		else if (cmd.getCmd() == "USER")
			user(client);
		else if (cmd.getCmd() == "PRIVMSG" && client.getNickname() != "")
			privmsg(client, cmd);
		else if (cmd.getCmd() == "MODE")
			mode(client, cmd);
		else if (cmd.getCmd() == "QUIT")
			return (quit(client, "QUIT"));
		else if (cmd.getCmd() == "TOPIC")
			topic(client, cmd);
		else if (cmd.getCmd() == "INVITE")
			invite(client, cmd);
		else if (cmd.getCmd() == "motd")
			motd(client);
		else if (cmd.getCmd() != "CAP" && cmd.getCmd() != "WHOIS" && cmd.getCmd() != "WHO")
			sendMessageToClient(client, ERR_UNKNOWNCOMMAND(client.getNickname(), cmd.getCmd()));
	}
	else
		sendMessageToClient(client, "Please enter the password and nick before start using the server's commands.");

	client.setMessage("");
	client.setNBytes(0);
	return (0);
}

void Server::pong(Client &client, Command &cmd)
{
	sendMessageToClient(client, "PONG " + cmd.getArg()[0]);
}



