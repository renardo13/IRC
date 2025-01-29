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
    bool isRegistered;
    int register_process;
    bool admin;
    double message_timer;

public:
    Client();
    ~Client();
    Client(Client const &obj);
    Client const &operator=(Client const &obj);
    Client(int socket);

    int getPfd() const;
    // std::vector<Channel> &getChannel();
    std::string getUsername() const;
    std::string getNickname() const;
    std::string getHostname() const;
    std::string getMessage();
    std::string getResMessage() const;
    bool getAdmin() const;
    int getRegisterProcess() const;
    int getNBytes() const;
    bool getIsRegistered() const;

    void setUsername(std::string username);
    void setNickname(std::string username);
    void setHostname(std::string username);
    void setMessage(std::string message);
    void setAdmin(bool flag);

    // void setChannel(std::vector<Channel> channels);
    void SetIsRegistered(bool status);
    void setNBytes(int);
    void setResMessage(std::string resMessage);
    void setRegisterProcess(int rp);

};

// Utils
int atoi(char *str);
std::string toStdString(char *str);

int sendMessageToClient(Client &client, std::string msg);
