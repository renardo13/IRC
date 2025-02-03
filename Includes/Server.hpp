#pragma once

#include "Client.hpp"
#include <map>
#include "Channel.hpp"

#define MAX_FDS 10
class Client;
class Channel;
class Command;

class Server
{
private:
    std::map<int, Client> clients;
    std::vector<Channel> channels;
    std::string pass;
public:
    Server();
    ~Server();
    Server(std::map<int, Client> &clients, std::vector<Channel> &channels);
    Server(Server const &obj);
    Server const &operator=(Server const &obj);

    std::map<int, Client> &getClients();
    std::vector<Channel> &getChannels();

    void setChannel(Channel& chan);
    void setClients(std::map<int, Client> &clients);

    int set_server(char *port, char *passwd);
    void print();
    void addNewClient(Client &client, int fd);
    int sendMessageToClient(Client &client, std::string msg);
    int sendMessageToEveryone(std::string msg, std::string chan);
    std::string getPassword() const;
    Client &getOneClientByNickname(std::string nick);
    void setPassword(std::string password);
    void handle_new_connection(int server_fd, struct pollfd pfds[], int *pfd_count);
    void handle_message(struct pollfd pfds[], int *pfd_count, int i);
    //commands member function
    void handle_commands(int fd);
    void join(Client &client, Command &cmd);
    void pong(Client &client, Command &cmd);
    void mode(Client &client, Command &cmd);
    void part(Client &client, Command &cmd);
    void kick(Client &client, Command &cmd);
    void password(Client &client, Command cmd, std::string server_pass);
    void nick(Client &client, Command cmd);
    void user(Client &client, Command cmd);
    void privmsg(Client &client, Command cmd);

};
