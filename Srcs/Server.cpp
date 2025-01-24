#include "Client.hpp"

void reuse_local_address(int server_fd)
{
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void bind_address(int server_fd)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        std::runtime_error("Error while binding\n");
}

void handle_new_connection(int server_fd, std::vector<struct pollfd> &pfds, std::map<int, Client> &clients)
{
    int new_socket = accept(server_fd, NULL, NULL);
    if (new_socket > 0)
    {
        if (pfds.size() > MAX_CLIENTS)
            std::cout << "Cannot add client to the server (server is full)\n";
        Client new_client(new_socket);
        clients.insert(std::pair<int, Client>(new_socket, new_client));
        struct pollfd new_pfd;
        new_pfd.fd = new_socket;
        new_pfd.events = POLLIN;
        pfds.push_back(new_pfd);
    }
}

void handle_message(std::vector<struct pollfd> &pfds, std::map<int, Client> &clients, int i)
{
    char buff[256];
    int nbytes = recv(pfds[i].fd, buff, 256, 0);
    buff[nbytes] = '\0';
    clients[i].setMessage(buff);
    if (nbytes <= 0)
    {
        if (nbytes == 0)
        {
            std::cout << "connection closed, client is removed" << std::endl;
            close(pfds[i].fd);
            clients.erase(clients.find(pfds[i].fd));
            pfds.erase(pfds.begin() + i);
        }
        else
            std::cerr << "Error receiving message" << std::endl;
    }
    else
    {
        std::cout << clients[i].getMessage();
    }
}

/* First create and fd witch a socket
Second initialize the struct sockaddr_in
After we need to link the struct with the socket with bind()*/
/* AF_INET for IPV4 adress familly */
/* SOCK_STREAM works with TCP protocol */

int set_server(char *port, char *passwd)
{
    std::map<int, Client> clients;
    std::vector<struct pollfd> pfds;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        std::runtime_error("Error while creating socket");
    reuse_local_address(server_fd);
    bind_address(server_fd);
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        close(server_fd);
        std::runtime_error("Error while listening");
    }
    struct pollfd serverpfd;
    serverpfd.fd = server_fd;
    serverpfd.events = POLLIN;
    pfds.push_back(serverpfd);
    pollfd *pfds_ptr;
    while (1)
    {
        pfds_ptr = &pfds[0];
        if (poll(pfds_ptr, pfds.size(), -1) < 0)
            std::runtime_error("poll error");
        for (int i = 0; i < (int)pfds.size(); i++)
        {
            if (pfds[i].revents & (POLLIN | POLLHUP))
            {
                if (pfds[i].fd == server_fd)
                    handle_new_connection(server_fd, pfds, clients);
                else
                    handle_message(pfds, clients, i);
            }
        }
    }
    close(server_fd);
}
