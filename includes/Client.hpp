#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <vector>
#include "Channel.hpp"
#include <sys/socket.h>

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
    int getClientFd();
    std::string getClientMsg(Client *client);
};

class ClientRecvException: public exception{

    public: virtual const char *what() const throw(){
        return ("Client disconnected");
    }

};

#endif