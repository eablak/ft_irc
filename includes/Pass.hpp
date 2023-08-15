#ifndef PASS_HPP
#define PASS_HPP

#include "ICommand.hpp"

class PASS : public ICommand {

    public:
    PASS();
    ~PASS();
    void execute(Server &server, Client &client);
};

#endif