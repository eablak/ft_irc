#ifndef MODE_HPP
#define MODE_HPP
#include "ICommand.hpp"

class Mode : public ICommand
{
private:
    void handleClient(Server &server, Client *client, std::vector<std::string> &params);
    void handleChannel(Server &server, Client *client, std::vector<std::string> &params);
    void handleMode(Server &server,Channel &channel ,Client *client , char mode);
public:
    Mode();
    ~Mode();
    void execute(Server &server, Client *client);
};
#endif
