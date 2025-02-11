#pragma once

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <signal.h>

#define RPL_JOIN "PASS " + psswd + "\r\n" + "NICK " + name +  "\r\n" + "USER " + name + " " + name + " " + name + "\r\n" + "JOIN #" + channel + "\r\n"

extern int g_sigint;

class Bot
{
    private:
        int pfd;
        // int server_pdf;
        int port;
        std::string channel;
        std::string psswd;
        std::string name;
        std::string serv_msg;
    public:
        Bot(int port, std::string channel, std::string psswd);
        ~Bot();
        int setBot();
        void setPfd(int pfd);
        int getPfd() const;
        // int sendMessageToClient(Client &client, std::string msg);
        // int sendMessageToEveryone(std::string msg, std::string chan);
        void connectToServer();
        void getMessages();
        void parseServMsg(std::string buffer);
        int signals();
};