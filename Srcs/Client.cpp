#include "../Includes/client.hpp"

Client::Client()
{
    std::cout << "A new client has beeen register" << std::endl;
}

Client::~Client() {}

Client::Client(Client const& obj)
{
    if(this != &obj)
    {
        *this = obj;
    }
}

Client const& Client::operator=(Client const& obj)
{
    this->name = obj.name;
    this->nickname = obj.nickname;
    this->fd = obj.fd;
    this->message_timer = obj.message_timer;
    return(*this);
}