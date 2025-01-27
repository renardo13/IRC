#ifndef MSG_GEN
#define MSG_GEN

#include <cstring>
#include "Client.hpp"

class Client;

std::string getMessageIsLongMessage(Client &client);
std::string getWelcomeMessage(Client &client);
#endif