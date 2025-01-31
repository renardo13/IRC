#ifndef RPL_MACROS_HPP
#define RPL_MACROS_HPP

//#define CRPL_PRIVMSG(client) (":" + client.getNickname + "!" + client.getNickname + "@host PRIVMSG " + "Bob" :Hello Bob!)
#define RPL_WELCOME(client) (":" + std::string("localhost") + " 001 " + (client).getNickname() + " :Welcome to the Internet Relay Network " + (client).getUsername())
#define CRPL_NICKCHANGE(oldnick,newnick) (":" + oldnick + " NICK " + newnick)
#define CMSG_PRIVMSG_CH(client, chname, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG #" + chname + " :" + msg)
#define CMSG_PRIVMSG_CL(client, client_target, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG " + client_target.getNickname() + " :" + msg)
#define ERR_ALREADYREGISTRED (":localhost 462 :You may not reregister")

#endif
