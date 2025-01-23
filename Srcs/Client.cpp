#include "../Includes/client.hpp"
#include<cstring>

Client::Client()
{
    std::cout << "A new client has beeen register" << std::endl;
}

Client::~Client() {}

Client::Client(int socket) : fd(socket)
{
    std::cout << "New client fd : " << socket << std::endl;
    memset(message, 0, sizeof(message));
};

Client::Client(Client const &obj)
{
    if (this != &obj)
    {
        *this = obj;
    }
}

Client const &Client::operator=(Client const &obj)
{
    this->name = obj.name;
    this->nickname = obj.nickname;
    this->fd = obj.fd;
    this->message_timer = obj.message_timer;
    return (*this);
}

void setMessage(char *message)
{
    this->message = message 
}

char* getMessage()
{

}