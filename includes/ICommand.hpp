#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

// #include "Pass.hpp"
// #include "Nick.hpp"
#include "Server.hpp"

class Server;
// class Pass;
// class Nick;

class ICommand
{

public:
    virtual ~ICommand(){};
    virtual void execute(Server &server, Client &client) = 0;
};

#endif