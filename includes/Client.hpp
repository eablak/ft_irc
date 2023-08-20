#ifndef CLIENT_HPP
#define CLIENT_HPP

class Server;

#include "libraries.hpp"
#include "Numeric.hpp"
#include "Channel.hpp"

enum _auth
{
    AUTHENTICATE,
    NOTAUTHENTICATED,
    REGISTERED
};

class Numeric;

class Client
{

private:
    int fd;
    std::string command;
    std::vector<std::string> params;
    _auth client_auth;
    std::list<std::pair<std::string, std::string> > client_message;
    std::string nickname;
    std::string username;
    std::string realname;
    std::vector<Channel> joinedChannels;

public:
    Client();
    Client(int fd);
    int getClientFd();
    _auth getAuthStatus();
    void setAuthStatus(_auth status);
    void setClientMessage(std::string, std::string);
    std::list<std::pair<std::string, std::string> > getMap();
    void printMap();
    void setParamsEnd();
    std::string getNickname();
    std::string getUsername();
    void setNickname(std::string _nickname);
    void setUsername(std::string _username);
    void setRealname(std::string _realname);
    std::string getCommand();
    std::vector<std::string> &getParams();
    void setParams(std::vector<std::string> tmp);
    void setCommand(std::string _command);
    bool isInChannel(std::string &channelName) const;
    void addChannel(Channel &channel);
    void removeChannel(Channel &channel);
};

#endif
