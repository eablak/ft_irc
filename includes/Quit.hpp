#ifndef QUIT_HPP
#define QUIT_HPP

#include "ICommand.hpp"
#include "../includes/Utils.hpp"

class Quit : public ICommand
{
public:
    Quit();
    ~Quit();
    void execute(Server &server, Client *client);
};

#endif