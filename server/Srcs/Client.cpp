#include "../Includes/Client.hpp"
#include <cstring>

Client::Client()
{
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
    isRegistered = false;
    isPasswordCorrect = false;
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
    this->isPasswordCorrect = obj.isPasswordCorrect;
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
    return (this->username);
}

std::string Client::getNickname() const
{
    return (this->nickname);
}

std::string Client::getHostname() const
{
    return (this->hostname);
}

int Client::getNBytes() const
{
    return nbytes;
}

int Client::getRegisterProcess() const
{
    return (this->register_process);
}

int Client::getNbChannels() const
{
    return (nb_channels);
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

void Client::IncreaseNbChannels()
{
    this->nb_channels++;
}

void Client::DecreaseNbChannels()
{
    this->nb_channels--;
}

std::vector<Client*>::iterator Client::getOperator(Channel &channel)
{
    std::vector<Client*>::iterator it = channel.getOperators().begin();
    for (int i = 1; it != channel.getOperators().end(); it++, i++)
    {
        if (getNickname() == (*it)->getNickname())
            return (it);
    }
    return (channel.getOperators().end());
}

std::vector<Client*>::iterator Client::getOperator(Channel &channel, std::string name)
{
    std::vector<Client*>::iterator it = channel.getOperators().begin();
    for (int i = 1; it != channel.getOperators().end(); it++, i++)
    {
        if (name == (*it)->getNickname())
            return (it);
    }
    return (channel.getOperators().end());
}

void Client::setRegisterProcess(int rp)
{
    this->register_process = rp;
}

void Client::setIsPasswordCorrect(bool status)
{
    this->isPasswordCorrect = status;
}
bool Client::getIsPasswordCorrect() const
{
    return this->isPasswordCorrect;
}