#pragma once 

#include "../Includes/Client.hpp"
#include <vector>

class Client;

class Channel
{
    private:
        std::string name;
        std::string psswd;
        std::string topic;
        bool invite_only;
        bool topic_op;
        int client_limit;
        std::vector<Client> clients;
        std::vector<std::string> operators;
        std::vector<std::string> invite_list;
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
        
        std::string getTopic() const;
        bool getInviteOnly() const;
        int getClientLimit() const;
        bool getTopicOp() const;

        void setPsswd(std::string key);
        void setClients(Client& client);
        void setClientLimit(int limit);
        void setInviteOnly(bool flag);
        void setTopic(std::string str);
        void setTopicOp(bool flag);
};