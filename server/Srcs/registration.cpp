#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

void Server::password(Client &client, std::string server_pass)
{
	if (client.getRegisterProcess() == 0)
	{
		std::string pass = client.getMessage().substr(client.getMessage().find(' ') + 1);
		if (pass == client.getMessage())
		{
			sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
			quit(client,CRPL_PASSWORD_ERROR);
		}
		else if (pass != server_pass)
		{
			sendMessageToClient(client, ERR_PASSWDMISMATCH);
			quit(client, CRPL_PASSWORD_ERROR);
			return ;
		}
		client.setMessage("");
		client.setIsPasswordCorrect(true);
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

std::string nicknameGenerator(std::string nickname)
{
	nickname += "_";
	if (nickname.size() > 9)
		return "";
	else
		return nickname;
}

void Server::nick(Client &client)
{
	int first_space = client.getMessage().find(' ');
	int second_space = client.getMessage().find(' ', first_space + 1);
	if (second_space != (int)std::string::npos)
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
	std::string nick = client.getMessage().substr(client.getMessage().find(' ') + 1);
	if (nick == client.getMessage())
	{
		sendMessageToClient(client, ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
		client.setMessage("");
		return;
	}
	if (isNickErroneous(nick))
	{
		sendMessageToClient(client, ERR_ERRONEUSNICKNAME(nick));
		client.setMessage("");
		return ;
	}
	if (isNickInUse(nick, getClients()) == 1)
	{
		if (client.getRegisterProcess() == 1)
		{
			sendMessageToClient(client, ERR_NICKNAMEINUSE(nick));
			client.setMessage("");
			return ;
		}
		else
		{
			std::string newNick = nicknameGenerator(nick);
			while(isNickInUse(newNick, getClients()) == 1 && newNick != "")
				newNick = nicknameGenerator(newNick);
			if (newNick != "")
				nick = newNick;
			else
			{
				quit(client, CRPLY_NICK_CHANGE_REQUIRED);
				return;
			}
		}
	}
	sendMessageToClient(client, CRPL_NICKCHANGE(client.getNickname(), nick));
	client.setNickname(nick);
	client.setHostname(client.getNickname() + "!" + client.getUsername());
	client.setMessage("");
}

void Server::user(Client &client)
{
	if (client.getRegisterProcess() == 0)
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
		client.setRegisterProcess(1);
		sendMessageToClient(client, RPL_WELCOME(client).c_str());
	}
	else
		sendMessageToClient(client, ERR_ALREADYREGISTRED);
}
