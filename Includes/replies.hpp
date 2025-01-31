#pragma once


/* --------------------------- Command reply -------------------------------------------- */

# define RPL_JOIN(hostname, channel) ":" + hostname + "@127.0.0.1 JOIN #" + channel + "\r\n"
# define CHAN_WELC(nickname, channel) nickname + " has joined #"  + channel

/* --------------------------- Error reply command -------------------------------------- */

# define ERR_NEEDMOREPARAMS(nickname, command) ":" + nickname + ":" + command + ":Not enough parameters given" + "\r\n"
# define ERR_NOSUCHCHANNEL(nickname, channel) ":localhost 403 " + nickname + " " + channel + " :No such channel"
