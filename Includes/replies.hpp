#pragma once

#include "Client.hpp"

/* --------------------------- Command reply -------------------------------------------- */

#define WLC(client) "001 " + client.getNickName() + " :" "Welcome to IRC server, " + client.getNickname() + "[!<" + user + ">@<localhost>]"
#define RPL_JOIN(client, channel_name) ":" + client.getNickname() + "@127.0.0.1 JOIN #" + channel_name + "\r\n"
#define CHAN_WELC(nickname, channel) nickname + " has joined #"  + channel
#define RPL_WELCOME(client) (":" + std::string("localhost") + " 001 " + (client).getNickname() + " :Welcome to the Internet Relay Network " + (client).getUsername())
#define RPL_PART(nickname, username, channel, part_msg) (":" + nickname + "!~" + username + "@127.0.0.1 PART #" + channel + " " + part_msg)
# define RPL_NAMES(nickname, channel, clientslist) (": 353 " + nickname + " @ #" + channel + " :" + clientslist + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel) (": 366 " + nickname + " #" + channel + " :END of /NAMES list")

/* --------------------------- Error reply command -------------------------------------- */

#define ERR_NEEDMOREPARAMS(nickname, command) ":" + nickname + ":" + command + ":Not enough parameters given" + "\r\n"
#define ERR_NOSUCHCHANNEL(nickname, channel) ":localhost 403 " + nickname + " " + channel + " :No such channel"
#define ERR_ALREADYREGISTRED (":localhost 462 :You may not reregister")
#define ERR_TOOMANYCHANNELS(nickname, channel) (":localhost 405 " + nickname + " :#" + channel)
#define ERR_TOOMANYCLIENTS(nickname, channel) (":irc_server 471 " + nickname + channel + " :Too many clients")
#define ERR_NOTONCHANNEL(client, channel) (":irc_server 442 " + client.getNickname() + " " + channel + " :You're not on that channel")
#define INVITE_ONLY(nickname, channel) (":localhost 473 " + nickname + " #" + channel + " :Cannot join channel (+i)")
#define ERR_NOTOPERATOR(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You're not channel operator")
#define ERR_PSSWD(nickname, channel) (":localhost 475 " + nickname + " #" + channel + " :Cannot join channel (+k) - bad key")

/* --------------------------- Custom reply -------------------------------------- */

#define CRPL_NICKCHANGE(oldnick,newnick) (":" + oldnick + " NICK " + newnick)

#define CMSG_PRIVMSG_CH(client, chname, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG #" + chname + " :" + msg)
#define CMSG_PRIVMSG_CL(client, client_target, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG " + client_target.getNickname() + " :" + msg)
