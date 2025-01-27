#include "../Includes/Client.hpp"
#include <cstring>

Client::Client()
{
    // std::cout << "A new client has been register" << std::endl;
}

Client::~Client() {}

Client::Client(int pfd) : pfd(pfd)
{
    // std::cout << "New connexion ! client pfd = " << pfd << std::endl;
    this->nbytes = 0;
    this->message = "";
    this->resMessage = "";
    this->isRegistered = false;
    this->username = "";
    this->hostname = "";
    this->nickname = "";
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

void Client::setMessage(std::string message)
{
    this->message = message;
}

std::string Client::getMessage()
{
    return (this->message);
}

std::string Client::getResMessage() const
{
    return (this->resMessage);
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

int Client::getNBytes() const
{
    return nbytes;
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

void Client::setNBytes(int nbytes)
{
    this->nbytes = nbytes;
}

void Client::setResMessage(std::string resMessage)
{
    this->resMessage = resMessage;
}

bool Client::getAdmin() const
{
    return(admin);
}

void Client::setAdmin(bool flag)
{
    admin = flag;
}

std::vector<Channel>& Client::getChannel()
{
    return(channels);
}

void Client::setChannel(std::vector<Channel> channels)
{
    this->channels = channels;
}