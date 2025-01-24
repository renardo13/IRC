#include "../Includes/Channel.hpp"

Channel::Channel() {};

Channel::~Channel() {};

Channel::Channel(Channel const& obj)
{
    if(this != &obj)
    {
        *this = obj;
    }
}

Channel const& Channel::operator=(Channel const& obj)
{
    this->name = obj.name;
    return(*this);
}

std::string Channel::getMode() const
{
    return(this->mode);
}

void Channel::setMode(std::string mode)
{
    this->mode = mode;
}
