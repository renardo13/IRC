#pragma once

#include "Client.hpp"
#include <map>
#include "Channel.hpp"
#include <stdio.h>

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
    struct pollfd pfds[MAX_FDS];
    int pfd_count;

public:
    Server();
    ~Server();
    Server(std::map<int, Client> &clients, std::vector<Channel> &channels);
    Server(Server const &obj);
    Server const &operator=(Server const &obj);

    std::map<int, Client> &getClients();
    std::vector<Channel> &getChannels();

    void setChannel(Channel &chan);
    void setClients(std::map<int, Client> &clients);

    int set_server(char *port, char *passwd);
    void print();
    void addNewClient(Client &client, int fd);
    int sendMessageToClient(Client &client, std::string msg);
    int sendMessageToEveryone(std::string msg, std::string chan);
    std::string getClientsList(Channel &chan);
    std::string getPassword() const;
    Client *getOneClientByNickname(std::string nick);
    void setPassword(std::string password);
    void handle_new_connection(int server_fd);
    void handle_message(int i);
    void deleteClientPfd(int i);
    int getPfdIndexByPfd(int pfd);
    int getCrlfAmount(const char *buff);
    int crlfCheck(const char *buff);
    void sendMessageToEveryClientInChannel(std::string msg, Channel &channel);
    // commands member function
    int handle_commands(int fd);
    int join(Client &client, Command &cmd);
    int reach_channel(Client &client, Command &cmd, Channel &chan, std::string chan_name);
    void create_channel(Client &client, std::string chan_name);
    void pong(Client &client, Command &cmd);
    int mode(Client &client, Command &cmd);
    int add_operator(Client &client, Command &cmd, Channel &chan);
    int remove_operator(Command &cmd, Channel &chan);
    int part(Client &client, Command &cmd);
    int kick(Client &client, Command &cmd);
    int set_limit(Channel &chan, Command &cmd);
    // int topic(Client &client, Command &cmd);
    void password(Client &client, std::string server_pass);
    void nick(Client &client);
    void user(Client &client);
    void privmsg(Client &client, Command &cmd);
    int quit(Client &client);
    int topic(Client &client, Command &cmd);
    void invite(Client &client, Command &cmd);
};
