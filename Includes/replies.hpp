#pragma once

#include "Client.hpp"

/* --------------------------- Command reply -------------------------------------------- */

#define WLC(client) "001 " + client.getNickName() + " :" "Welcome to IRC server, " + client.getNickname() + "[!<" + user + ">@<localhost>]"
#define RPL_JOIN(hostname, channel_name) ":" + hostname + "@127.0.0.1 JOIN #" + channel_name + "\r\n"
#define CHAN_WELC(nickname, channel) nickname + " has joined #"  + channel
#define RPL_WELCOME(client) (":" + std::string("localhost") + " 001 " + (client).getNickname() + " :Welcome to the Internet Relay Network " + (client).getUsername())
#define RPL_PART(nickname, username, channel, part_msg) (":" + nickname + "!~" + username + "@127.0.0.1 PART #" + channel + " " + part_msg)
#define RPL_NAMES(nickname, channel, clientslist) (": 353 " + nickname + " @ #" + channel + " :" + clientslist + "\r\n")
#define RPL_ENDOFNAMES(nickname, channel) (": 366 " + nickname + " #" + channel + " :END of /NAMES list")
#define RPL_KICK(nickname, username, channel, person_kicked, reason) (":" + nickname + "!~" + username + "@127.0.0.1 KICK #" + channel + " " + person_kicked + " :" + reason)

// For every modes
# define RPL_CHANGEMODE(hostname, channel, mode, args) (":" + hostname + " MODE #" + channel + " " + mode + " " + args)

/* --------------------------- Error reply command -------------------------------------- */

#define ERR_NEEDMOREPARAMS(nickname, command) ":" + nickname + ":" + command + ":Not enough parameters given"
#define ERR_NOSUCHCHANNEL(nickname, channel) ":localhost 403 " + nickname + " " + channel + " :No such channel"
#define ERR_ALREADYREGISTRED (":localhost 462 client :You may not reregister")
#define ERR_TOOMANYCHANNELS(nickname, channel) (":localhost 405 " + nickname + " :#" + channel)
#define ERR_TOOMANYCLIENTS(nickname, channel) (":localhost 471 " + nickname + " #" + channel + " :Cannot join channel (+l)")
#define ERR_NOTONCHANNEL(client, channel) (":irc_server 442 " + client.getNickname() + " " + channel + " :You're not on that channel")
#define INVITE_ONLY(nickname, channel) (":localhost 473 " + nickname + " #" + channel + " :Cannot join channel (+i)")
#define ERR_NOTOPERATOR(nickname, channel) (":localhost 482 " + nickname + " #" + channel + " :You're not channel operator")
#define ERR_PSSWD(nickname, channel) (":localhost 475 " + nickname + " #" + channel + " :Cannot join channel (+k) - bad key")

#define ERR_NOSUCHNICK(client, nick) (":localhost 401 " + client.getNickname() + " " + nick + " :No such nick")
#define ERR_NOTEXTTOSEND(client) (":localhost 412 " + client.getNickname() + " :No text to send")
#define ERR_NORECIPIENT(client) (":localhost 411 " + client.getNickname() + " :No recipient is given")
#define ERR_NICKNAMEINUSE(nick) (":localhost 433 client " + nick + " :Nickname is already in use")
#define ERR_PASSWDMISMATCH (":localhost 464 client :Password is incorrect")
# define ERR_UNKNOWNMODE(nickname, char) (":localhost 472 " + nickname + " :" + char)
/* --------------------------- Custom reply -------------------------------------- */

#define CRPL_NICKCHANGE(oldnick,newnick) (":" + oldnick + " NICK " + newnick)

#define CMSG_PRIVMSG_CH(client, chname, msg) (":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getHostname() + " PRIVMSG #" + chname + " :" + msg)
#define RPL_PRIVMSG(nickname, target, msg) (":" + nickname + " PRIVMSG " + target + " :" + msg)
