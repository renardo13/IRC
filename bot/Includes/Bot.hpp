#pragma once

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include "../../server/Includes/Command.hpp"
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

/* ------------------------------------------------------BOT REPLIES-------------------------------------------------------------------------------------------- */
#define RPL_JOIN "PASS " + psswd + "\r\n" + "NICK " + name +  "\r\n" + "USER " + name + " " + name + " " + name + "\r\n" + "JOIN #" + channel
#define RPL_IN_CHAN(chan, msg) "PRIVMSG #" + chan + " :" + msg
#define RPL_KICK(channel, person_kicked, reason) ("KICK #" + channel + " " + person_kicked + " :" + reason)
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------- */

extern int g_sigint;

class Bot
{
    private:
        int pfd;
        struct pollfd fds[1];
        struct sockaddr_in server_addr;
        int port;
        std::string channel;
        std::string psswd;
        std::string name;
        std::string serv_msg;
        bool admin;
        bool play_mode;
        int adversary_point;
        int bot_point;
        int start;
    public:
        Bot(int port, std::string channel, std::string psswd);
        ~Bot();
        int setBot();
        void setPfd(int pfd);
        int getPfd() const;
        int sendMsgToServer(std::string msg);
        void connectToServer();
        void getMessages();
        void play(std::string user);
        void parseServMsg(std::string buffer);
        bool amIAdmin(std::string buffer);
        std::string randomChoice();
        void dual(std::string command);
        bool isOpenedSocket(int socket);
};