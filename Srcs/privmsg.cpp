#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

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
			sendMessageToClient(client, ERR_NOSUCHNICK(client.getNickname(), target_client_nickname));
			return;
		}
		sendMessageToClient(*target_client, RPL_PRIVMSG(client.getNickname(), target_client->getNickname(), msgval));
	}
	else
	{
		while (it_chname != chan_names.end())
		{
			Channel* it_ch = findValue(getChannels(),
															 &Channel::getName, *it_chname);
			if (it_ch != NULL)
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
