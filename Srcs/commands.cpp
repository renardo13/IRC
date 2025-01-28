#include "../Includes/Client.hpp"
int sendMessageToClient(Client &client, std::string msg);

void handle_commands(Server& server, int fd)
{
    Client &client = server.getClients()[fd];
    std::string msg = client.getMessage();
    std::string cmd = msg.substr(0, msg.find(' '));
    std::cout << cmd << std::endl;
    if (cmd == "PASS")
    {
        if (client.getRegisterProcess() == 0)
        {
            std::string pass = msg.substr(msg.find(' ') + 1);
            client.setRegisterProcess(1);
            //pass auth
        }
    }
    else if (cmd == "NICK")
    {
        if (client.getRegisterProcess() == 1)
        {
            std::string nick = msg.substr(msg.find(' ') + 1);
            client.setNickname(nick);
            client.setRegisterProcess(2);
        }
        else
        {
            client.setRegisterProcess(0);
        }  
    }
    else if (cmd == "USER")
    {
        if (client.getRegisterProcess() == 2)
        {
            std::string user = msg.substr(msg.find(' ') + 1);
            client.setUsername(user); 
            client.SetIsRegistered(true);
            client.setRegisterProcess(3);
            sendMessageToClient(client,getWelcomeMessage(client).c_str());
        }
        else
           client.setRegisterProcess(0); 
    }
    else
        sendMessageToClient(server.getClients()[fd], "Unknown command");
    client.setMessage("");
    client.setNBytes(0);
}

void join(Server &server)
{

   (void)server;
}