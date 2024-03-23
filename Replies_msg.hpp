#ifndef REPLIES_MSG_HPP
#define REPLIES_MSG_HPP

#define RPL_WELCOME(nick, hostname) ":" + hostname + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname) ":" + hostname + " 002 " + nick + " :Your host is " + hostname + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname) ":" + hostname + " 003 " + nick + " :This server was created 2024-02-21 !\r\n"
#define RPL_MYINFO(nick, hostname) ":" + hostname + " 004 " + nick + " :Host: " + hostname + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i, l !\r\n"

#define ERR_PASSWDMISMATCH(nick, hostname) ":" + hostname + " 464 " + nick + " :Password incorrect !\r\n"
#define ERR_ALREADYREGISTERED(nick, hostname) ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"
#define ERR_NOTREGISTERED(nick, hostname) ":" + hostname + " 451 " + nick + " :You have not registered !\r\n"

#define ERR_ERRONEUSNICKNAME(nick, hostname) ":" + hostname + " 432 " + nick + " :Erroneus nickname !\r\n"
#define ERR_NONICKNAMEGIVEN(nick, hostname) ":" + hostname + " 431 " + nick + " :No nickname given !\r\n"
#define ERR_NICKNAMEINUSE(nick, hostname) ":" + hostname + " 433 " + nick + " :Nickname is already in use !\r\n"
#define RPL_NICKCHANGE(oldNick, nick, hostname) ":" + oldNick + " NICK " + nick + "\r\n"

#define ERR_ALREADYREGISTERED(nick, hostname) ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"
#define ERR_NEEDMOREPARAMS(nick, hostname) ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n"

#define ERR_CHANNELISFULL(nick, channelName) ":" + nick + " 471 " + channelName + " :Cannot join channel, Channel is full (+l)\r\n"
#define ERR_BADCHANNELKEY(nick, hostname, channelName) ":" + hostname + " 475 " + nick + " " + channelName + " :Cannot join channel, you need the correct key (+K)\r\n"
#define ERR_INVITEONLY(nick, channelName) ":" + nick + " 473 " + channelName + " :Cannot join channel, you must be invited (+i)\r\n"
#define ERR_NORECIPIENT(nick, hostname, command) ":" + hostname + " 411 " + nick + " :No recipient given " + "(" + command + ")" + "\r\n"


#define RPL_JOIN(nick, username, channelname, ipaddress) ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define RPL_TOPIC(nick, hostname, channelname, topic) ":" + hostname + " 332 " + nick + " " + channelname + " :" + topic + "\r\n"

#define RPL_NAMREPLY(hostname, clients, channelname, nick) ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients
#define RPL_ENDOFNAMES(hostname, nick, channelname) ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list.\r\n"


#define ERR_USERNOTINCHANNEL(hostname, channel) ":" + hostname + " 441 " + channel + " " + ":they aren't on that channel\r\n"
#define ERR_NOTOP(hostname, channel) ":" + hostname + " 482 " + channel + " " + ":You're not a channel operator\r\n"

#define ERR_NOSUCHCHANNEL(hostname, channel, nick) ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NOSUCHNICK(hostname, channel, argument) ":" + hostname + " 401 " + channel + " " + argument + " :No such nick/channel\r\n"
#define ERR_USERONCHANNEL(hostname, channel, nick) ":" + hostname + " 443 " + nick + " " + channel + "  :is already on channel\r\n"
#define ERR_NOTEXTTOSEND(hostname, nick) ":" + hostname + " 412 " + nick + " :No text to send\r\n"
#define ERR_CANNOTSENDTOCHAN(hostname, channel, nick) ":" + hostname + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n"

#define ERR_UNKNOWNMODE(nick, hostname, channel, character) ":" + hostname + " 472 " + nick + " " + channel + " " + character + " :is unknown mode char to me\r\n"
#define ERR_UNKNOWNCOMMAND(nick, hostname, command) ":" + hostname + " 421 " + nick + " " + command + " :Unknown command\r\n"

#endif