#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "libraries.hpp"
// #include "Client.hpp"
// #include "Server.hpp"

// #define RPL_NAMEREPLY(nick, channel, users) ("353", "= " + channel + " :" + users)
// #define ERR_NOSUCHNICK(nick) Numeric::createNumeric("401", nick + " :No such nick/channel")
// #define ERR_NOSUCHCHANNEL(channel) Numeric::createNumeric("403", channel + " :No such channel")
// #define ERR_USERNOTINCHANNEL(nick, channel) Numeric::createNumeric("441", nick + " " + channel + " :They aren't on that channel")
// #define ERR_USERONCHANNEL(nick, channel) Numeric::createNumeric("443", nick + " " + channel + " :is already on channel")
// #define ERR_NOTREGISTERED Numeric::createNumeric("451", ":You have not registered")
// #define ERR_CHANOPRIVSNEEDED(channel) Numeric::createNumeric("482", channel + " :You're not channel operator")

#define RPL_TOPIC(channel, topic) "332 " + channel + " :" + topic + "\r\n"
#define RPL_NOTOPIC(channel) "331 " + channel +  "\r\n"
#define RPL_WELCOME(nick, user, host) ("001 :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host + "\r\n")
#define RPL_YOURHOST(nick, host) (std::string("002 ") + ":Your host is " + host + ", running version 1.0 \r\n")
#define RPL_CREATED(nick, date) (std::string("003 ") + ":This server was created " + date + "\r\n")
#define RPL_NAMREPLY(channel, nicknames) "353 " + channel + " : " + nicknames + "\r\n"
#define RPL_ENDOFNAMES(channel) "366 " + channel + " :End of NAMES list" + "\r\n"

#define ERR_NEEDMOREPARAMS(cmd) (std::string("461 ") + cmd + " :Not enough parameters\r\n")
#define ERR_INPUTTOOLONG() ("417 :Input line was too long\r\n")
#define ERR_PASSWDMISMATCH() ("464 :Password incorrect\r\n")
#define ERR_NONICKNAMEGIVEN() ("431 :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(nick) ("432 " + nick + " :Erroneus nickname\r\n")
#define ERR_UNKNOWNCOMMAND(cmd) ("421 " + cmd + " :Unknown command\r\n")
#define ERR_ALREADYREGISTRED() ("462 :You may not reregister\r\n") // USER veya PASS'i bi daha gönderirse
#define ERR_NICKNAMEINUSE(nick) ("433 " + nick + " :Nickname is already in use\r\n")
#define ERR_NOSUCHNICK(nickname) "401 " + nickname + " :No such nick/channel" + "\r\n"
#define ERR_NOSUCHCHANNEL(channel) "403 " + channel + " :No such channel" + "\r\n"
#define ERR_NOTONCHANNEL(channel) "442 " + channel + " :You're not on that channel" + "\r\n"
#define ERR_USERNOTINCHANNEL(nick, channel) "441 " + nick + " " + channel + " :They aren't on that channel" + "\r\n"
#define ERR_CHANOPRIVSNEEDED(channel) "482 " + channel + " :You're not channel operator" + "\r\n"
class Client;
class Server;

class Numeric
{
public:
    static void printNumeric(Client *client, Server &server, std::string errMessage);
};

#endif
