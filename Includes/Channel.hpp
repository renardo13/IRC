#pragma once 

#include "../Includes/Client.hpp"

class Channel
{
    private:
        std::string name;
        std::string mode;
    public:
        Channel();
        ~Channel();
        Channel(Channel const& obj);
        Channel const& operator=(Channel const& obj);
        std::string getMode() const;
        void setMode(std::string mode);
};