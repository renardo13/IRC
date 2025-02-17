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
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "colors.hpp"

#define MAX_CLIENTS 512
#define MAX_PORT 65535
#define MAX_CHANNEL_FOR_CLIENT 10

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
    bool isPasswordCorrect;
    int register_process;
    bool admin;
    int nb_channels;
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
    int getRegisterProcess() const;
    int getNBytes() const;
    bool getIsRegistered() const;
    bool getIsPasswordCorrect() const;
    std::vector<Client*>::iterator getOperator(Channel& chan);
    std::vector<Client*>::iterator getOperator(Channel &channel, std::string name);
    void setUsername(std::string username);
    void setNickname(std::string username);
    void setHostname(std::string username);
    void setMessage(std::string message);
    void IncreaseNbChannels();
    void DecreaseNbChannels();

    void SetIsRegistered(bool status);
    void setIsPasswordCorrect(bool status);
    void setNBytes(int);
    void setResMessage(std::string resMessage);
    void setRegisterProcess(int rp);
    int getNbChannels() const;

};

// Utils
int atoi(char *str);
std::string toStdString(char *str);
std::string getRealNickname(std::string nick);
bool passwordCheck(std::string pass);