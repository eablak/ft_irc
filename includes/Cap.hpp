#ifndef CAP_HPP
#define CAP_HPP

#include "ICommand.hpp"
#include "../includes/Server.hpp"
class Cap : public ICommand
{
public:
    Cap();
    ~Cap();
    void execute(Server &server, Client *client);
};
#endif