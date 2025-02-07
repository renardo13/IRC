#include "../Includes/Client.hpp"
#include <sstream>
#include <stdio.h>

std::string Server::getClientsList(Channel &chan)
{
    std::string list = "";
    std::vector<Client>::iterator it = chan.getClients().begin();
    for (int i = 0; it != chan.getClients().end(); it++, i++)
    {
        if (i != 0)
            list += ' ';
        if (it->is_operator(chan))
            list += '@';
        list += it->getNickname();
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


