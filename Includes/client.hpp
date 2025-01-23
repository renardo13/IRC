#pragma once

#include <iostream>

class Client
{
    private:
        int fd;
        std::string name;
        std::string nickname;
        double message_timer;
    public:
        Client();
        ~Client();
        Client(Client const& obj);
        Client const& operator=(Client const& obj);
        Client(int socket) : fd(socket) {};

};