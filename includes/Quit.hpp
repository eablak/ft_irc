#ifndef QUIT_HPP
#define QUIT_HPP

#include "ICommand.hpp"

class Quit : public ICommand
{
    public:
        Quit();
        ~Quit();
        void execute(Server &server, Client &client);
};

#endif