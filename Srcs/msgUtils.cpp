#include "Client.hpp"
#include "Server.hpp"
int sendMessageToClient(Client &client, std::string msg)
{
    std::string irc_msg = msg + "\r\n";
    send(client.getPfd(), irc_msg.c_str(),irc_msg.size(),0);
    return 1;
}
