#pragma once

#include "Client.hpp"

/* --------------------------- Command reply -------------------------------------------- */

#define RPL_JOIN(client, channel) ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " JOIN #" + channel + "\r\n"
#define CHAN_WELC(nickname, channel) nickname + " has joined #"  + channel
#define RPL_WELCOME(client) (":" + std::string("localhost") + " 001 " + (client).getNickname() + " :Welcome to the Internet Relay Network " + (client).getUsername())
#define RPL_PART(client, channel) (":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 PART #" + channel)

/* --------------------------- Error reply command -------------------------------------- */

#define ERR_NEEDMOREPARAMS(nickname, command) ":" + nickname + ":" + command + ":Not enough parameters given" + "\r\n"
#define ERR_NOSUCHCHANNEL(nickname, channel) ":localhost 403 " + nickname + " " + channel + " :No such channel"
#define ERR_ALREADYREGISTRED (":localhost 462 :You may not reregister")

/* --------------------------- Custom reply -------------------------------------- */

#define CRPL_NICKCHANGE(oldnick,newnick) (":" + oldnick + " NICK " + newnick)