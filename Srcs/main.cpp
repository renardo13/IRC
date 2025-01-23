#include "Client.hpp"
#include "Server.hpp"

int main()
{
    try
    { 
        set_server();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
