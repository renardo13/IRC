#ifndef RPL_MACROS_HPP
#define RPL_MACROS_HPP

#define CRPL_NICKCHANGE(oldnick,newnick) (":" + oldnick + " NICK " + newnick)
#define RPL_WELCOME(client) (":" + std::string("localhost") + " 001 " + (client).getNickname() + " :Welcome to the Internet Relay Network " + (client).getUsername())
#define ERR_ALREADYREGISTRED (":localhost 462 :You may not reregister")

#endif
