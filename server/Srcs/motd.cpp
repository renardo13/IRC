#include "Server.hpp"
#include "replies.hpp"

void Server::motd(Client &client)
{
	std::string motd_str;
	std::ifstream motd_fd;
	motd_fd.open(motd_file.c_str());
	if (motd_fd.is_open())
		motd_fd >> motd_str;
	else
	{
		sendMessageToClient(client, ERR_NOMOTD(client));
		return ;
	}
	sendMessageToClient(client, RPL_MOTDSTART(client));
	while (motd_fd.good())
	{
		sendMessageToClient(client, RPL_MOTD(client, motd_str));
		motd_fd >> motd_str;
	}
	sendMessageToClient(client, RPL_ENDOFMOTD(client));

}