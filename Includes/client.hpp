#pragma once

#include <iostream>

class Client
{
    private:
        int fd;
        char message[512];
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