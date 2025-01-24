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
#include "../Includes/Client.hpp"

#define MAX_CLIENTS 1024
#define MAX_PORT 65535

class Client
{
    private:
        int pfd;
        char *message;
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
        char* getMessage();

};


int atoi(char *str);