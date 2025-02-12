#include "../Includes/Bot.hpp"
#include "../../server/Includes/colors.hpp"
#include "../../server/Includes/Command.hpp"

class Command;

Bot::Bot(int port, std::string channel, std::string psswd) : port(port), channel(channel), psswd(psswd), name("Julia") {}

Bot::~Bot() {}

void Bot::connectToServer()
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(pfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        throw std::runtime_error("Bot failed to connect to server");
    std::string mess = RPL_JOIN;
    send(pfd, mess.c_str(), mess.size(), 0);
}

void Bot::parseServMsg(std::string buffer)
{
    (void)buffer;
    Command cmd;
    cmd.parseCmd(buffer);
    std::cout << BOLD << cmd.getMode()[0] << std::endl;
}

void Bot::getMessages()
{
    char buffer[512];
    int nbytes = 0;
    while (!g_sigint)
    {
        nbytes = recv(pfd, buffer, 512, 0);
        if (nbytes == -1)
            throw std::runtime_error("Error while receiving data from server");
        buffer[nbytes] = '\0';
        // std::cout << BOLD << RED << "[SERVER RESPONSE] => " << buffer << RESET << std::endl;
        // parseServMsg(buffer);
    }
    std::string quit = "QUIT :leaving + \r\n";
    send(pfd, quit.c_str(), sizeof(quit), 0);
    exit(1);
}

int Bot::setBot()
{
    pfd = socket(AF_INET, SOCK_STREAM, 0);
    connectToServer();
    getMessages();
    return (0);
}

void Bot::setPfd(int pfd)
{
    this->pfd = pfd;
}

int Bot::getPfd() const
{
    return (pfd);
}

// int Bot::sendMessageToClient(Client &client, std::string msg)
// {
//     std::string irc_msg = msg + "\r\n";
//     std::cout << BOLD << RED << "[BOT RESPONSE] => " << msg << RESET << std::endl;
//     return (send(client.getPfd(), irc_msg.c_str(), irc_msg.size(), 0));
// }

// int Bot::sendMessageToEveryone(std::string msg, std::string chan_name)
// {
//     for (std::vector<Client *>::iterator clients = chan->getClients().begin(); clients != chan->getClients().end(); clients++)
//         sendMessageToClient(*(*clients), msg);
//     return (0);
// }