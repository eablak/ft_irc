#ifndef WHO_HPP
#define WHO_HPP
#include "ICommand.hpp"

class Who : public ICommand
{
public:
    Who();
    ~Who();
    void execute(Server &server, Client *client);
};
#endif