#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "ICommand.hpp"

class Topic : public ICommand
{
    public:
        Topic();
        ~Topic();
        void execute(Server &server, Client &client);
};

#endif