#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <vector>
#include "Channel.hpp"
using namespace std;

class Channel;

class Client{

    private:
    int fd;
    string nickname;
    string username;
    vector<Channel *> _channels;

    public:
    Client(int fd);
    int checkPass(string _pass);
    int getClientFd(Client *client);
};


#endif