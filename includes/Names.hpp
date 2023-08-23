#ifndef NAMES_HPP
#define NAMES_HPP
#include "libraries.hpp"
#include "Utils.hpp"
#include "ICommand.hpp"
class Names : public ICommand
{

private:
    void handleMultipleChannels(Server &server, Client *client);
    void handleWithParams(Server &server, Client *client, std::vector<std::string> &params);
public:
    Names();
    ~Names();
    void execute(Server &server, Client *client);
};
#endif