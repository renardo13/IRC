#pragma once

#include "Client.hpp"
#include <map>
#include "Channel.hpp"
#define MAX_FDS 10
class Client;
class Channel;

class Server
{
private:
    std::map<int, Client> clients;
    std::vector<Channel> channels;

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

    //commands member function
    void handle_commands(int fd);
    void join(Client &client);
    void part(Client &client);
};
