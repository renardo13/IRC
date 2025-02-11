#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

void Server::password(Client &client, std::string server_pass)
{
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

int isNickInUse(std::string nick, std::map<int, Client>& clients)
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
	int first_space = client.getMessage().find(' ');
	int second_space = client.getMessage().find(' ', first_space + 1);
	if (second_space != (int)std::string::npos)
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
	std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
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
		client.SetIsRegistered(true);
		client.setRegisterProcess(3);
		sendMessageToClient(client, RPL_WELCOME(client).c_str());
	}
	else if (client.getRegisterProcess() > 2)
		sendMessageToClient(client, ERR_ALREADYREGISTRED);
}
