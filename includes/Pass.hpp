#ifndef PASS_HPP
#define PASS_HPP

#include "ICommand.hpp"

class Pass : public ICommand
{

public:
    Pass();
    ~Pass();
    void execute(Server &server, Client *client);
};

#endif