#pragma once

#include "Client.hpp"

/* --------------------------- Command reply -------------------------------------------- */

#define RPL_JOIN(client, channel) ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " JOIN #" + channel + "\r\n"
#define CHAN_WELC(nickname, channel) nickname + " has joined #"  + channel
#define RPL_WELCOME(client) (":" + std::string("localhost") + " 001 " + (client).getNickname() + " :Welcome to the Internet Relay Network " + (client).getUsername())
#define RPL_PART(client, channel) (":" + client.getNickname() + "!~" + client.getUsername() + "@127.0.0.1 PART " + channel + " :Leaving")

/* --------------------------- Error reply command -------------------------------------- */

#define ERR_NEEDMOREPARAMS(nickname, command) ":" + nickname + ":" + command + ":Not enough parameters given" + "\r\n"
#define ERR_NOSUCHCHANNEL(nickname, channel) ":localhost 403 " + nickname + " " + channel + " :No such channel"
#define ERR_ALREADYREGISTRED (":localhost 462 :You may not reregister")
#define ERR_TOOMANYCHANNELS(nickname, channel) (":localhost 405 " + nickname + " :#" + channel)
#define ERR_NOTONCHANNEL(client, channel) (":irc_server 442 " + client.getNickname() + " " + channel + " :You're not on that channel")

/* --------------------------- Custom reply -------------------------------------- */

#define CRPL_NICKCHANGE(oldnick,newnick) (":" + oldnick + " NICK " + newnick)

#define CMSG_PRIVMSG_CH(client, chname, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG #" + chname + " :" + msg)
#define CMSG_PRIVMSG_CL(client, client_target, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG " + client_target.getNickname() + " :" + msg)
#define ERR_ALREADYREGISTRED (":localhost 462 :You may not reregister")
