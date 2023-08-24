#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "ICommand.hpp"
#include "Utils.hpp"
class Topic : public ICommand
{
private:
    void sendTopicToChannel(Server &server,  Channel &channel);
public:
    Topic();
    ~Topic();
    void execute(Server &server, Client *client);
};

#endif