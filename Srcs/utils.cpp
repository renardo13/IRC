#include "../Includes/Client.hpp"
#include <sstream>

std::string Server::getClientsList(Channel &chan)
{
    std::vector<Client>::iterator it = chan.getClients().begin();
    std::string list;

    for (int i = 0; it != chan.getClients().end(); it++, i++)
    {
        if (i != 0)
            list += ' ';
        list += it->getNickname();
    }
    std::cout << "LIST = " << list << std::endl;
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
