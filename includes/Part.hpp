#ifndef PART_HPP
#define PART_HPP
#include "ICommand.hpp"
#include "../includes/libraries.hpp"
#include "../includes/Utils.hpp"
class Part : public ICommand
{
private:
    void handleMultipleChannels(Server &server, Client *client, std::vector<std::string> &params);
    void handleWithParams(Server &server, Client *client, std::vector<std::string> &params);
    void partFromAllChannels(Server &server, Client *client);
    void checkLastOperator(Server &server, Client *client, Channel &ch);
public:
    Part();
    ~Part();
    void execute(Server &server, Client *client);
};
#endif