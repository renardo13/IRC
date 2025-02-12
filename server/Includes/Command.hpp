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

        void parseCmd(std::string message);

        std::string getCmd() const {return cmd;};
        std::vector<std::string>& getChannel() {return channel;};
        std::vector<std::string>& getUser() {return user;};
        std::string getMsg() const {return msg;};
        std::vector<std::string>& getMode() {return mode;};
};