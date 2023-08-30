#ifndef MODE_HPP
#define MODE_HPP
#include "ICommand.hpp"

class Mode : public ICommand
{
private:
    void handleChannel(Server &server, Client *client, std::vector<std::string> &params);
public:
    Mode();
    ~Mode();
    void execute(Server &server, Client *client);
};
#endif
