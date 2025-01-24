#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> // for sockaddr_in struct
#include <unistd.h>     // fd gestion
#include <cstring>
#include <poll.h>
#include <map>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include "error.hpp"
#include "../Includes/Client.hpp"

#define MAX_CLIENTS 1024
#define MAX_PORT 65535

class Client
{
    private:
        int pfd;
        std::string message;
        std::string name;
        std::string nickname;
        double message_timer;
    public:
        Client();
        ~Client();
        Client(Client const& obj);
        Client const& operator=(Client const& obj);
        Client(int socket);

        void setMessage(char *message);
        std::string getMessage();
        int getPfd() const;

};


int atoi(char *str);
std::string getWelcomeMessage(Client &client);
int sendMessageToClient(Client &client, std::string msg);