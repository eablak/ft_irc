#ifndef MODE_HPP
#define MODE_HPP
#include "ICommand.hpp"

class Mode : public ICommand
{
private:
    void handleChannel(Server &server, Client *client, std::vector<std::string> &params);
    void addModes(Server &server, Channel &channel, Client *client, std::string &modes);
    void removeModes(Server &server, Channel &channel, Client *client, std::string &modes);
    void handleModes(Server &server, Channel &channel, Client *client, char mode);

public:
    Mode();
    ~Mode();
    void execute(Server &server, Client *client);
};
#endif
