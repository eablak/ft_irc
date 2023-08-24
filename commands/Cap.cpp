#include "../includes/Cap.hpp"

Cap::Cap()
{
}

Cap::~Cap()
{
}

void Cap::execute(Server &server, Client *client)
{
    server.messageToClient(client->getClientFd(), "CAP * LS :multi-refix sasl");
}