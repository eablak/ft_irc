#ifndef CLIENT_HPP
#define CLIENT_HPP

class Server;

#include "librarys.hpp"

class Client{

    private:
    int fd;
    enum _auth {AUTHENTICATE, NOTAUTHENTICATED, REGISTERED};
    _auth client_auth;
   
    public:
    Client();
    Client(int fd);
    int getClientFd();
};

#endif