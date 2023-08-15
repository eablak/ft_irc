#ifndef HANDLEMESSAGE_HPP
#define HANDLEMESSAGE_HPP

#include "Server.hpp"
#include "ICommand.hpp"
#include "Pass.hpp"

class Server;
class Client;
class ICommand;

class handleMessage{

    private:
    // ICommand'ekle
    std::map<std::string, ICommand *> _commandMap;

    public:
    int handleMsg(Server &server, Client &client, std::string msg);
    void clientMsgProcess(Server &server, Client &client);
    void processNotAuthenticated(Server &server, Client &client);
    void processAuthenticate(Server &server, Client &client);
    void processRegistered(Server &server, Client &client);
    void setCommandMap(std::string, ICommand command);
    std::map<std::string, ICommand *> getCommandMap();
};

#endif
