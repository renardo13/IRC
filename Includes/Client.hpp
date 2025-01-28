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
#include <algorithm>
#include "../Includes/Client.hpp"
#include "../Includes/Channel.hpp"
#include "msgGenerator.hpp"

#define MAX_CLIENTS 1024
#define MAX_PORT 65535

class Server;
class Channel;

class Client
{
private:
    int pfd;
    int nbytes;
    std::string message;
    std::string resMessage;
    std::string username;
    std::string nickname;
    std::string hostname;
    std::vector<Channel> channels;
    bool isRegistered;
    bool admin;
    double message_timer;

public:
    Client();
    ~Client();
    Client(Client const &obj);
    Client const &operator=(Client const &obj);
    Client(int socket);

    int getPfd() const;
    std::string getUsername() const;
    std::string getNickname() const;
    std::string getHostname() const;
    std::string getMessage();
    std::string getResMessage() const;
    std::vector<Channel> &getChannel();
    bool getAdmin() const;
    int getNBytes() const;
    bool getIsRegistered() const;

    void setUsername(std::string username);
    void setNickname(std::string username);
    void setHostname(std::string username);
    void setMessage(std::string message);
    void setChannel(std::vector<Channel> channels);
    void SetIsRegistered(bool status);
    void setNBytes(int);
    void setAdmin(bool flag);
    void setResMessage(std::string resMessage);
};

// Utils
int atoi(char *str);
std::string toStdString(char *str);

// commands
void handle_commands(Server &server, int fd);
void join(Server &server, Client &client);
void leave(Server &server, Client &client);

int sendMessageToClient(Client &client, std::string msg);
