#pragma once 

#include <iostream>
#include "../Includes/Client.hpp"

class Command
{
    private:
        std::string cmd;
        std::vector<std::string> channel;
        std::vector<std::string> user;
        std::string msg;
        std::vector<std::string> mode;
    public:
        Command() {};
        ~Command() {};
        Command(Command const &obj); 
        Command const &operator=(Command const &obj);

        Command parseCmd(std::string message);
};