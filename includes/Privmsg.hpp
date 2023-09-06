#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP
#include "Utils.hpp"
#include "ICommand.hpp"
class Privmsg : public ICommand
{

private:
    void handleMultipleTargets(Server &server, Client *client, std::vector<std::string> params);
    void handleWithParams(Server &server, Client *client, std::vector<std::string> params);
    void sendMessageToChannel(Server &server, Client *client, std::vector<std::string> params);
    void sendMessageToClient(Server &server, Client *client, std::vector<std::string> params);

public:
    Privmsg();
    ~Privmsg();
    void execute(Server &server, Client *client);
};

#endif