#include "../Includes/Client.hpp"

void handle_commands(std::string buff)
{
    std::string cmd = buff.substr(0, buff.find(' '));
    if(cmd == "JOIN")
        join();
    // if(cmd == "NICK")
    //     nick();
    // ...
    
}

void join()
{

}