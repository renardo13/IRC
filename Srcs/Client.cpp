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
    this->isRegistered = false;
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
    this->username = obj.username;
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

bool Client::getIsRegistered() const
{
    return (this->isRegistered);
}

std::string Client::getUsername() const
{
    return(this->username);
}

std::string Client::getNickname() const
{
    return(this->nickname);
}

std::string Client::getHostname() const
{
    return(this->hostname);
}

void Client::setUsername(std::string username)
{
    this->username = username;
}

void Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

void Client::setHostname(std::string hostname)
{
    this->hostname = hostname;
}

void Client::SetIsRegistered(bool status)
{
    this->isRegistered = status;
}

