#include "../Includes/Client.hpp"
#include <cstring>


Client::Client()
{
    std::cout << "A new client has been register" << std::endl;
}

Client::~Client() {}

Client::Client(int pfd) : pfd(pfd)
{
    nbytes = 0;
    message = "";
    resMessage = "";
    username = "";
    nickname = "";
    hostname = "";
    isRegistered = "";
    admin = "";
    register_process = 0;
    nb_channels = 0;
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
    this->pfd = obj.pfd;
    this->message_timer = obj.message_timer;
    this->nbytes = obj.nbytes;
    this->message = obj.message;
    this->resMessage = obj.resMessage;
    this->isRegistered = obj.isRegistered;
    this->username = obj.username;
    this->hostname = obj.hostname;
    this->nickname = obj.nickname;
    this->register_process = obj.register_process;
    this->admin = obj.admin;
    this->nb_channels = obj.nb_channels;
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

int Client:: getRegisterProcess() const
{
    return (this->register_process);
}

int Client::getNbChannels() const
{
    return(nb_channels);
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

void Client::IncreaseNbChannels()
{
    this->nb_channels++;
}


void Client::DecreaseNbChannels()
{
    this->nb_channels--;
}

int Client::is_operator(Channel& channel)
{
    std::vector<std::string>::iterator it = channel.getOperators().begin();
    for (; it != channel.getOperators().end(); it++)
    {
        if (this->getNickname() == *it)
            return (1);
    }
    return (0);
}

void Client::setRegisterProcess(int rp)
{
    this->register_process = rp;
}
