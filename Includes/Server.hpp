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
        Server(std::map<int, Client>& clients);
        Server(Server const &obj);
        Server const &operator=(Server const &obj);

        std::map<int, Client>& getClients();
        void setClients(std::map<int, Client> &clients);
        void print_clients(Server& server);
};

int set_server(char *port, char *passwd);