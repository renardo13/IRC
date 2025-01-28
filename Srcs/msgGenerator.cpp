#include <cstring>
#include "Client.hpp"

std::string getWelcomeMessage(Client &client)
{
    std::string nickname = client.getNickname();
    std::string user = client.getUsername();;
    std::string hostname = "localhost";
    (void)client;
    return (":" + hostname + " 001 " + nickname + " :Welcome to the Internet Relay Network " + user);
}

std::string getMessageIsLongMessage(Client &client)
{
    (void)client;
    return ("Messages cannot contain more than 512 characters (included CRLF)");
}