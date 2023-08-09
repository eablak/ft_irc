#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <vector>
#include "Channel.hpp"
#include <sys/socket.h>
#include <map>

using namespace std;

class Channel;
class Server;

typedef std::map<std::string, bool> MapAuths;

class Client{

    private:
    int fd;
    bool auth;
    MapAuths map_auth[3];
    string nickname;
    string username;
    string realname;
    vector<Channel *> _channels;

    public:
    Client(int fd);
    int checkPass(string _pass);
    int getClientFd();
    std::string getClientMsg(Client *client);
    bool getAuth();
    void NotAuthClient(Client *client, Server *server, string msg);
};

class ClientRecvException: public exception{

    public: virtual const char *what() const throw(){
        return ("Client disconnected");
    }

};

#endif