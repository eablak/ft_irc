#ifndef USER_HPP
#define USER_HPP

#include "ICommand.hpp"
#include "Utils.hpp"
class User : public ICommand
{

public:
    User();
    ~User();
    void execute(Server &server, Client *client);
};

#endif