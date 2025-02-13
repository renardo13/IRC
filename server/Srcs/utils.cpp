#include "../Includes/Client.hpp"
#include <sstream>

void Server::sendMessageToEveryClientInChannel(std::string msg, Channel &channel)
{
	std::vector<Client *>::iterator client_it = channel.getClients().begin();
	for (; client_it != channel.getClients().end(); client_it++)
		sendMessageToClient(*(*client_it), msg);
}

int Server::sendMessageToEveryone(std::string msg, std::string chan_name)
{
    std::vector<Channel>::iterator chan = isChannelInServer(chan_name);
    if (chan == getChannels().end())
        return (-1);
    for (std::vector<Client *>::iterator clients = chan->getClients().begin(); clients != chan->getClients().end(); clients++)
        sendMessageToClient(*(*clients), msg);
    return (0);
}

int Server::getCrlfAmount(const char *buff)
{
    int i = 0;
    int n = 0;
    while (buff[i])
    {
        if (buff[i] == '\r' && buff[i + 1] == '\n')
            n++;
        i++;
    }
    return n;
}

int Server::crlfCheck(const char *buff)
{
    int i = 0;
    while (buff[i])
    {
        if (buff[i] == '\r' && buff[i + 1] == '\n')
            return i;
        i++;
    }
    return -1;
}

std::string Server::getClientsList(Channel &chan)
{
    std::string list = "";
    std::vector<Client *>::iterator it = chan.getClients().begin();
    for (int i = 0; it != chan.getClients().end(); it++, i++)
    {
        if (i != 0)
            list += ' ';
        if ((*it)->getOperator(chan) != chan.getOperators().end())
            list += '@';
        list += (*it)->getNickname();
    }
    return (list);
}


int atoi(char *str)
{
    std::istringstream iss(str);
    int result;

    iss >> result;
    return (result);
}

std::string toStdString(char *str)
{
    std::istringstream iss(str);
    std::string result;

    iss >> result;
    return (result);
}

std::string getRealNickname(std::string nick)
{
    if (nick[0] == '@')
    {
        return (nick.substr(1));
    }
    else
        return nick;
}

int Server::getPfdIndexByPfd(int pfd)
{
    int i = 0;
    while (i < MAX_FDS - 1)
    {
        if (pfds[i].fd == pfd)
            return i;
        i++;
    }
    return 0;
}

bool isStrAlnum(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if (!isalnum(str[i]))
            return false;
        i++;
    }
    return true;
}

bool isNickErroneous(std::string nick)
{
    return (nick.find(' ') != std::string::npos || nick.find('@') != std::string::npos || nick == "" || !isStrAlnum(nick));
}

std::vector<Client*>::iterator Channel::isClientInChan(Client& client)
{
    for(std::vector<Client*>::iterator it = getClients().begin(); it != getClients().end(); it++)
    {
        if((*it)->getNickname() == client.getNickname())
            return(it);
    }
    return(getClients().end());
}


std::vector<Client*>::iterator Channel::isClientInChan(std::string name)
{
    for(std::vector<Client*>::iterator it = getClients().begin(); it != getClients().end(); it++)
    {
        if((*it)->getNickname() == name)
            return(it);
    }
    return(getClients().end());
}