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
#include "Server.hpp"
#include "../Includes/Client.hpp"

#define MAX_CLIENTS 1024
#define MAX_PORT 65535

class Client
{
    private:
        int pfd;
        std::string message;
        std::string username;
        std::string nickname;
        std::string hostname;
        bool isRegistered;
        // bool admin;
        double message_timer;
    public:
        Client();
        ~Client();
        Client(Client const& obj);
        Client const& operator=(Client const& obj);
        Client(int socket);

        int getPfd() const;
        std::string getUsername() const;
        std::string getNickname() const;
        std::string getHostname() const;
        std::string getMessage();
        bool getIsRegistered() const;
        
        void setUsername(std::string username);
        void setNickname(std::string username);
        void setHostname(std::string username);
        void setMessage(char *message);
        void SetIsRegistered(bool status);

};

//Utils
int atoi(char *str);
std::string toStdString(char *str);

//commands
void handle_commands(std::string buff);
void join();

std::string getWelcomeMessage(Client &client);
int sendMessageToClient(Client &client, std::string msg);