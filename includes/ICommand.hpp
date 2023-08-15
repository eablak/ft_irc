#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class Server;

class ICommand{

    public:
    virtual ~ICommand(){};
    virtual void run(Server &server, Client &client, std::list<std::pair<std::string, 
        std::string> >client_message) = 0;
};

#endif