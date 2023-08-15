#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include "ICommand.hpp"
#include "Server.hpp"

class Server;

class ICommand{

    public:
    virtual ~ICommand(){};
    virtual void execute(Server &server, Client &client) = 0;
};

#endif