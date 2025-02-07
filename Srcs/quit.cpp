#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

int Server::quit(Client &client)
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
	return(0);
}
