#ifndef HANDLEMESSAGE_HPP
#define HANDLEMESSAGE_HPP

#include "Server.hpp"

class handleMessage{

    public:
    int handleMsg(Server &server, Client &client, std::string msg);
    void clientMsgProcess(Server &server, Client &client);
    void processNotAuthenticated(Server &server, Client &client);
    void processAuthenticate(Server &server, Client &client);
    void processRegistered(Server &server, Client &client);
};

#endif
