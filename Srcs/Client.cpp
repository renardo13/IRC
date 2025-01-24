#include "../Includes/Client.hpp"
#include <cstring>

Client::Client()
{
    std::cout << "A new client has been register" << std::endl;
}

Client::~Client() {}

Client::Client(int pfd) : pfd(pfd)
{
    std::cout << "New connexion ! client pfd = " << pfd << std::endl;
    memset(&message, 0, sizeof(message));
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
    this->pfd = obj.pfd;
    this->message_timer = obj.message_timer;
    return (*this);
}

void Client::setMessage(char *message)
{
    this->message = message;
}

std::string Client::getMessage()
{
    return (this->message);
}

int Client::getPfd() const
{
    return (this->pfd);
}