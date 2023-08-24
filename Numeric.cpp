#include "includes/Numeric.hpp"
#include "includes/Server.hpp"

void Numeric::printNumeric(Client *client, Server &server, std::string errMessage)
{
    server.messageToClient(client, client, errMessage);
}
