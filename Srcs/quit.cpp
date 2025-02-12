#include "../Includes/Server.hpp"
#include "../Includes/replies.hpp"

int Server::quit(Client &client, std::string msg)
{
    sendMessageToClient(client,ERROR(msg));
    int tmp_pfd = client.getPfd();

    std::vector<Channel>::iterator chan = getChannels().begin();
    while (chan != getChannels().end())
    {
        std::vector<Client*>::iterator chan_client = chan->getClients().begin();
        bool client_found = false;

        while (chan_client != chan->getClients().end())
        {
            if ((*chan_client)->getNickname() == client.getNickname())
            {
                chan->getClients().erase(chan_client);
                client_found = true;
                break;
            }
            ++chan_client;
        }
        if (client_found && chan->getClients().empty())
            chan = getChannels().erase(chan);
        else
            ++chan;
    }
	int tmp_fd = 0;
    std::map<int, Client>::iterator client_it = getClients().begin();
    while (client_it != getClients().end())
    {
        if (client_it->second.getNickname() == client.getNickname())
        {
            tmp_fd = client_it->first;
            getClients().erase(client_it);
            break;
        }
        ++client_it;
    }
    if(tmp_pfd)
        close(tmp_pfd);
    deleteClientPfd(getPfdIndexByPfd(tmp_pfd));
    return 0;
}
