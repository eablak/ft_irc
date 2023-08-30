#ifndef PING_HPP
#define PING_HPP

#include "ICommand.hpp"
#include "../includes/Utils.hpp"

class Ping : public ICommand
{
public:
    Ping();
    ~Ping();
    void execute(Server &server, Client *client);
};

#endif