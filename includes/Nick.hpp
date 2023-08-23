#ifndef NICK_HPP
#define NICK_HPP

#include "ICommand.hpp"

class Nick : public ICommand
{

public:
    Nick();
    ~Nick();
    void execute(Server &server, Client *client);
};

#endif