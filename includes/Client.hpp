#ifndef CLIENT_HPP
#define CLIENT_HPP

class Server;

#include "librarys.hpp"
#include "Numeric.hpp"

enum _auth {AUTHENTICATE, NOTAUTHENTICATED, REGISTERED};

class Numeric;

class Client{

    private:
    int fd;
    std::string current_msg;
    _auth client_auth;
    std::list<std::pair<std::string, std::string> >client_message;
    Numeric _nums;
   
    public:
    Client();
    Client(int fd);
    int getClientFd();
    void setMsg(std::string msg);
    _auth getAuthStatus();
    void setAuthStatus(_auth status);
    void setClientMessage(std::string, std::string);
    std::list<std::pair<std::string, std::string> > getMap();
    void printMap();
    void setMapSecondEnd();
    Numeric &getNums();
};

#endif