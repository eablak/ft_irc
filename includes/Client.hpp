#ifndef CLIENT_HPP
#define CLIENT_HPP

class Server;

#include "librarys.hpp"

class Client{

    private:
    int fd;
    enum _auth {AUTHENTICATE, NOTAUTHENTICATED, REGISTERED};
    _auth client_auth;
    std::string current_msg;
   
    public:
    Client();
    Client(int fd);
    int getClientFd();
    void setMsg(std::string msg);
};

#endif