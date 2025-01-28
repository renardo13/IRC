#pragma once 

#include "../Includes/Client.hpp"
#include <vector>

class Client;

class Channel
{
    private:
        std::string name;
        std::string mode;
        std::vector<Client> clients;
    public:
        Channel();
        ~Channel();
        Channel(Channel const& obj);
        Channel const& operator=(Channel const& obj);
        Channel(std::string name);

        std::string getMode() const;
        std::string getName() const;
        std::vector<Client>& getClients();

        void setMode(std::string mode);
        void setClients(Client& client);
};