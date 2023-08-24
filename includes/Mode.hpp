#ifndef MODE_HPP
#define MODE_HPP
#include "ICommand.hpp"

class Mode : public ICommand
{
public:
    Mode();
    ~Mode();
    void execute(Server &server, Client *client);
};