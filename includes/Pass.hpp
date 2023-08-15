#ifndef PASS_HPP
#define PASS_HPP

#include "ICommand.hpp"

class PASS : public ICommand {

    public:
    PASS();
    ~PASS();
    void run(Server &server, Client &client, std::list<std::pair<std::string, 
        std::string> >client_message);
};

#endif