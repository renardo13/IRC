#pragma once

#include "Client.hpp"
#include <map>

class Client;

class Server
{
    private:
        std::map<int, Client> clients;
        std::vector<std::string> 
    public:
        Server();
        ~Server();
        Server(std::map<int, Client> clients);
        Server(Server const& obj);
        Server const &operator=(Server const& obj);
};

int set_server(char *port, char *passwd);