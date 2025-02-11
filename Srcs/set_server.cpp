#include "Client.hpp"
#include "Server.hpp"

extern int isRunning;

void reuse_local_address(int server_fd)
{
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void bind_address(int port, int server_fd)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;

    if (port < 0 || port > MAX_PORT)
        throw std::runtime_error(ERR_PORT_NOT_VALID_RANGE);
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error(ERR_SOCKET_BINDING);
}

void Server::deleteClientPfd(int i)
{
    while (i < MAX_FDS - 1)
    {
        pfds[i] = pfds[i + 1];
        i++;
    }
}

void Server::handle_new_connection(int server_fd)
{
    int new_socket = accept(server_fd, NULL, NULL);
    if (new_socket > 0)
    {
        if (pfd_count > MAX_FDS - 1)
        {
            send(new_socket,"Server is full.",16,0);
            close(new_socket);
            return ;
        }
        Client new_client(new_socket);
        addNewClient(new_client, new_socket);
        struct pollfd new_pfd;
        new_pfd.fd = new_socket;
        new_pfd.events = POLLIN;
        new_pfd.revents = 0;
        pfds[pfd_count] = new_pfd;
        pfd_count += 1;
    }
}

void Server::handle_message(int i)
{
    char buff[512];
    std::map<int, Client>& clients = getClients();
    int nbytes = recv(pfds[i].fd, buff, 512, 0);
    clients[pfds[i].fd].setNBytes(clients[pfds[i].fd].getNBytes() + nbytes);
    if (nbytes <= 0)
    {
        if (nbytes == 0)
        {
            std::cout << "connection closed, client is removed";
            close(pfds[i].fd);
            clients.erase(clients.find(pfds[i].fd));
            deleteClientPfd(i);
            pfd_count-=1;
        }
        else
            throw std::runtime_error(ERR_SOCKET_RECEIVE);
    }
    else
    {
        //TO-DO: Add long command check (>512)
        buff[nbytes] = '\0';
        std::string buff_str = std::string(buff);
        int nCrlf = getCrlfAmount(buff_str.c_str());
        if (nCrlf != 0)
        {
            while (nCrlf != 0)
            {
                int limiter = crlfCheck(buff_str.c_str());
                clients[pfds[i].fd].setResMessage(buff_str.substr(limiter + 1));
                clients[pfds[i].fd].setMessage(clients[pfds[i].fd].getMessage() + buff_str.substr(0, limiter));
                handle_commands(pfds[i].fd);
                buff_str = buff_str.substr(limiter + 2);
                nCrlf = getCrlfAmount(buff_str.c_str());
            }
        }
        else
            clients[pfds[i].fd].setMessage(clients[pfds[i].fd].getMessage() + buff_str);
    }
}

/* First create and fd witch a socket
Second initialize the struct sockaddr_in
After we need to link the struct with the socket with bind()*/
/* AF_INET for IPV4 adress familly */
/* SOCK_STREAM works with TCP protocol */

int Server::set_server(char *port, char *passwd)
{
    setPassword(passwd);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        throw std::runtime_error(ERR_SOCKET_CREATION);
    reuse_local_address(server_fd);
    bind_address(atoi(port), server_fd);
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        close(server_fd);
        throw std::runtime_error(ERR_SOCKET_LISTENING);
    }
    struct pollfd serverpfd;
    serverpfd.fd = server_fd;
    serverpfd.events = POLLIN;
    pfds[0] = serverpfd;
    pfd_count = 1;
    while (isRunning)
    {
        if ((poll(pfds, pfd_count, -1)) < 0)
        {
            if (isRunning == 1)
                throw std::runtime_error(ERR_POLL_FAILURE);        
        }
        for (int i = 0; i < pfd_count; i++)
        {
            int flags = fcntl(pfds[i].fd, F_GETFL, 0);
            if (flags == -1) {
                throw std::runtime_error("fcntl F_GETFL failed");
            }
            if (fcntl(pfds[i].fd, F_SETFL, flags | O_NONBLOCK) == -1) {
                throw std::runtime_error("fcntl F_SETFL failed");
            }
            if (pfds[i].revents & (POLLIN | POLLHUP))
            {
                if (pfds[i].fd == server_fd)
                    handle_new_connection(server_fd);
                else
                    handle_message(i);
            }
        }
    }   
    closeAllClientPfds();
    close(server_fd);
    return 1;
}
