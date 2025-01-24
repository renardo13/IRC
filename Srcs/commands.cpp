#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"
#include <stdio.h>

void handle_commands(std::string buff, Server& server)
{
    std::string cmd = buff.substr(0, buff.find(' '));
    if(cmd == "JOIN")
        join(server);
    // if(cmd == "NICK")
    //     nick();
    // ...
    
}

//implementer jooin dans la class serveur pour y avoir acces
void join(Server& server)
{
      const std::map<int, Client>& clients = server.getClients();

    // Utilisation de l'itérateur sur la référence
    std::map<int, Client>::iterator it = clients.begin();

    // Affichage de la clé et de la valeur pointées par l'itérateur
    if (it != clients.end()) {
        std::cout << "Key: " << it->first << ", Client: " << it->second.getMessage() << std::endl;
    }
    

    (void)server;
    

}