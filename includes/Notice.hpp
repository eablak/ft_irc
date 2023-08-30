#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "ICommand.hpp"
#include "Utils.hpp"

class Notice : public ICommand
{
public:
    Notice();
    ~Notice();
    void channelOrUser(Server &server, Client *client, std::vector<std::string> params);
    void sendToMessageChannel(Server &server, Client *client, std::vector<std::string> params);
    void sendToMessageUser(Server &server, Client *client, std::vector<std::string> params);
    void multipleTargets(Server &server, Client *client, std::vector<std::string> params);
    void execute(Server &server, Client *client);
};

#endif