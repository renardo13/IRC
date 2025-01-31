#include "../Includes/Client.hpp"
#include <sstream>

int atoi(char *str)
{
    std::istringstream iss(str);
    int result;
    
    iss >> result;
    return (result);
}

std::string toStdString(char *str)
{
    std::istringstream iss(str);
    std::string result;
    
    iss >> result;
    return (result);
}

std::string getRealNickname(std::string nick)
{
    if (nick[0] == '@')
    {
        return (nick.substr(1));
    }
    else
        return nick;
}

