#include "../Includes/Client.hpp"
#include <sstream>

int atoi(char *str)
{
    std::istringstream iss(str);
    int result;
    
    iss >> result;
    return (result);
}