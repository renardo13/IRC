#include <cstring>
#include "Client.hpp"

std::string getWelcomeMessage(Client &client)
{
    //hard coded client properties
    std::string nickname = "test_nickname";
    std::string user = "test_user";
    std::string hostname = "localhost";
    (void)client;
    return (":" + hostname + " 001 " + nickname + " :Welcome to the Internet Relay Network " + user);
}

std::string getMessageIsLongMessage(Client &client)
{
    (void)client;
    return ("Messages cannot contain more than 512 characters (included CRLF)");
}