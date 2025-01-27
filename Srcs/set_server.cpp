#include "Client.hpp"
#include "Server.hpp"
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

void handle_new_connection(int server_fd, std::vector<struct pollfd> &pfds, Server &server)
{
    int new_socket = accept(server_fd, NULL, NULL);
    if (new_socket > 0)
    {
        if (pfds.size() > MAX_CLIENTS)
            throw std::runtime_error(ERR_SERVER_FULL);
        Client new_client(new_socket);
        server.addNewClient(new_client,new_socket);
        struct pollfd new_pfd;
        new_pfd.fd = new_socket;
        new_pfd.events = POLLIN;
        pfds.push_back(new_pfd);
        sendMessageToClient(new_client,getWelcomeMessage(new_client).c_str());
    }
}

int crlfCheck(char buff[512])
{
    int i = 0;
    while (buff[i])
    {
        if (buff[i] == '\r' && buff[i + 1] == '\n')
            return i;
        i++;
    }
    return -1;
}

void handle_message(std::vector<struct pollfd> &pfds, Server& server, int i)
{
    char buff[512];
    std::map<int, Client>& clients = server.getClients();
    int nbytes = recv(pfds[i].fd, buff, 512, 0);
    clients[pfds[i].fd].setNBytes(clients[pfds[i].fd].getNBytes() + nbytes);
    if (nbytes <= 0)
    {
        if (nbytes == 0)
        {
            std::cout << "connection closed, client is removed";
            close(pfds[i].fd);
            clients.erase(clients.find(pfds[i].fd));
            pfds.erase(pfds.begin() + i);
        }
        else
            throw std::runtime_error(ERR_SOCKET_RECEIVE);
    }
    else
    {
        if (clients[pfds[i].fd].getNBytes() > 512)
        {
            clients[pfds[i].fd].setMessage("");
            clients[pfds[i].fd].setResMessage("");
            clients[pfds[i].fd].setNBytes(0);
            sendMessageToClient(clients[pfds[i].fd],getMessageIsLongMessage(clients[pfds[i].fd]).c_str());
            return ;
        }
        buff[nbytes] = '\0';
        std::string buff_str = std::string(buff);
        int limiter = crlfCheck(buff);
        if (limiter == -1)
        {
            clients[pfds[i].fd].setMessage(clients[pfds[i].fd].getMessage() + buff_str);
            std::cout << "CRLF not found buffer set to: " << clients[pfds[i].fd].getMessage()  << std::endl;
        }
        else
        {
            clients[pfds[i].fd].setResMessage(buff_str.substr(limiter + 1));
            clients[pfds[i].fd].setMessage(clients[pfds[i].fd].getMessage() + buff_str.substr(0, limiter));
            std::cout << "CRLF found full message is: " << clients[pfds[i].fd].getMessage() << std::endl;
            std::cout << "Rest of the message is: " << clients[pfds[i].fd].getResMessage() << std::endl;
            handle_commands(server, pfds[i].fd);
        }
    }
}

/* First create and fd witch a socket
Second initialize the struct sockaddr_in
After we need to link the struct with the socket with bind()*/
/* AF_INET for IPV4 adress familly */
/* SOCK_STREAM works with TCP protocol */

int set_server(char *port, char *passwd)
{
    (void)passwd;
    std::map<int, Client> clients;
    Server server(clients);
    std::vector<struct pollfd> pfds;

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
    pfds.push_back(serverpfd);
    pollfd *pfds_ptr;
    while (1)
    {
        pfds_ptr = &pfds[0];
        if (poll(pfds_ptr, pfds.size(), -1) < 0)
            throw std::runtime_error(ERR_POLL_FAILURE);
        for (int i = 0; i < (int)pfds.size(); i++)
        {
            if (pfds[i].revents & (POLLIN | POLLHUP))
            {
                if (pfds[i].fd == server_fd)
                {
                    handle_new_connection(server_fd, pfds, server);
                }
                else
                {
                    handle_message(pfds, server, i);
                }
            }
        }
    }
    close(server_fd);
}
