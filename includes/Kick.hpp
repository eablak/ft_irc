#ifndef KICK_HPP
#define KICK_HPP
#include "ICommand.hpp"
#include "Utils.hpp"
#include <typeinfo>
class Kick : public ICommand
{
public:
    Kick();
    ~Kick();
    void execute(Server &server, Client *client);
};
#endif
