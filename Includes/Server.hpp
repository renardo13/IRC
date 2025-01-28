#pragma once

#include "Client.hpp"
#include <map>
#include "Channel.hpp"

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
        Server(std::map<int, Client>& clients, std::vector<Channel>& channels);
        Server(Server const &obj);
        Server const &operator=(Server const &obj);
        std::map<int, Client>& getClients();
        std::vector<Channel>& getChannels();
        void setClients(std::map<int, Client> &clients);
        void print();
        void addNewClient(Client &client, int fd);

};

int set_server(char *port, char *passwd);
void handle_commands(Server& server, Client &client);