#pragma once

#include "Client.hpp"
#include <map>
#include "Channel.hpp"
#include "rplMacros.hpp"

#define MAX_FDS 10
class Client;
class Channel;

class Server
{
private:
    std::map<int, Client> clients;
    std::vector<Channel> channels;
    std::string pass;
public:
    Server();
    ~Server();
    Server(std::map<int, Client> &clients, std::vector<Channel> &channels);
    Server(Server const &obj);
    Server const &operator=(Server const &obj);

    std::map<int, Client> &getClients();
    std::vector<Channel> &getChannels();

    void setChannel(Channel& chan);
    void setClients(std::map<int, Client> &clients);

    int set_server(char *port, char *passwd);
    void print();
    void addNewClient(Client &client, int fd);
    int sendMessageToClient(Client &client, std::string msg);
    std::string getPassword() const;

    void setPassword(std::string password);
    //commands member function
    void handle_commands(int fd);
    void join(Client &client);
    void part(Client &client);
    void kick(Client &client);
    void quit(Client &client);
};
