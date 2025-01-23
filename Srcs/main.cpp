#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> // for sockaddr_in struct
#include <unistd.h>     // fd gestion
#include <cstring>
#include <map>
#include "Includes/client.hpp"

#define PORT 15080
#define MAX_CLIENTS 10

/* First create and fd witch a socket
Second initialize the struct sockaddr_in
After we need to link the struct with the socket with bind()*/
/* AF_INET for IPV4 adress familly */
/* SOCK_STREAM works with TCP protocol */
int main()
{
    // std::map<int, Client> clients;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        std::cout << "Error while creating socket\n";
    }
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY; 

    /* Casting in a generic struct sockaddr because struct sockaddr_in is for IPV4 address */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "Error while binding\n";
    }
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        std::cout << "Error while listening\n";
        close(server_fd);
    }
    while (1)
    {
        int new_socket = accept(server_fd, NULL, NULL);
        if(new_socket < 0)
            continue;
        char buffer[512];
        memset(buffer, 0, sizeof(buffer));
        int nread = read(new_socket, &buffer, sizeof(buffer));
        if(nread > 0)
        {
            std::cout << buffer;
        }
        // parsing(buffer);
        close(new_socket);
    }

    // close the server_fd
    close(server_fd);
   
}