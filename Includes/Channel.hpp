#pragma once 

#include "../Includes/Client.hpp"
#include <vector>

class Client;

class Channel
{
    private:
        std::string name;
        std::string psswd;
        bool invite_only;
        int client_limit;
        std::vector<Client> clients;
        std::vector<std::string> operators;
        
    public:
        Channel();
        ~Channel();
        Channel(Channel const& obj);
        Channel const& operator=(Channel const& obj);
        Channel(std::string name);

        std::string getPsswd() const;
        std::string getName() const;
        std::vector<Client>& getClients();
        std::vector<std::string>& getOperators();
        bool getInviteOnly() const;
        bool getClientLimit() const;

        void setPsswd(std::string key);
        void setClients(Client& client);
        void setClientLimit(bool flag);
        void setInviteOnly(bool flag);

};