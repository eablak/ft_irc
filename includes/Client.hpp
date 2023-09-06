#ifndef CLIENT_HPP
#define CLIENT_HPP

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

class Server;
class Client
{

private:
    int fd;
    std::string command;
    std::vector<std::string> params;
    _auth client_auth;
    std::string nickname;
    std::string username;
    std::string realname;
    std::vector<Channel> joinedChannels;
    std::string hostname;
    std::string message;

public:
    Client();
    Client(int fd, std::string hostname);
    int getClientFd();
    _auth getAuthStatus();
    void setAuthStatus(_auth status);
    void setParamsEnd();
    std::string getNickname();
    std::string getUsername();
    void setNickname(std::string _nickname);
    void setUsername(std::string _username);
    void setRealname(std::string _realname);
    std::string getRealname();
    std::string getCommand();
    std::vector<std::string> &getParams();
    void setParams(std::vector<std::string> tmp);
    void setCommand(std::string _command);
    bool isInChannel(std::string &channelName) const;
    void addChannel(Channel &channel);
    void removeChannel(Channel &channel);
    std::vector<Channel> &getChannels();
    std::string getPrefix();
    void setMessage(std::string message);
    std::string getMessage();
};

#endif
