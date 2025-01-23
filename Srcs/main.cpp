#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h> // for sockaddr_in struct
#include <unistd.h>     // fd gestion
#include <cstring>
#include <poll.h>
#define PORT 17080
#define MAX_CLIENTS 10

/* First create and fd witch a socket
Second initialize the struct sockaddr_in
After we need to link the struct with the socket with bind()*/
/* AF_INET for IPV4 adress familly */
/* SOCK_STREAM works with TCP protocol */
int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int newFd;
    //int fdSize = 5;
    char buf[256];
    if (server_fd == -1)
    {
        std::cout << "Error while creating socket\n";
    }
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY; 

    struct pollfd pfds[5];
    pfds[0].fd = server_fd;
    pfds[0].events = POLLIN;
    int fdCount = 1;
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
        int poll_count = poll(pfds, fdCount, -1);

        if (poll_count == -1) 
            std::cerr << "poll error" << std::endl;
        for (int i = 0; i < fdCount; i++)
        {
            if (pfds[i].revents & (POLLIN | POLLHUP))
            {
                if (pfds[i].fd == server_fd)
                {
                    newFd = accept(server_fd, NULL, NULL);
                    if (newFd > 0)
                    {
                        if (fdCount == 5)
                            std::cout << "Cannot add client to the server (server is full)" << std::endl;
                        std::cout << "New Connection!" << std::endl;
                        const char* welcomeMsg = "Hello from server";
                        send(newFd, welcomeMsg, strlen(welcomeMsg), 0);
                        pfds[fdCount].fd = newFd;
                        pfds[fdCount].events = POLLIN;
                        fdCount++;
                    }
                    else
                        std::cout << "Ann error occured while accepting new client" << std::endl;
                }
                else
                {
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
                    //int sender_fd = pfds[i].fd;
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0)
                        {
                            std::cout << "connection closed" << std::endl;
                            close(pfds[i].fd);
                            fdCount--;
                        }
                        else
                            std::cout << "Error receiving msg" << std::endl;
                    }
                    else
                    {
                        for (int j = 0; j < nbytes; j++)
                        {
                            std::cout << buf[j];
                        }
                    }
                }
            }
        }
    }

    // close the server_fd
    close(server_fd);
   
}