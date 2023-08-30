#include "../includes/Ping.hpp"

Ping::Ping(){};
Ping::~Ping(){};

void Ping::execute(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();
    params = Utils::concatParams(params);
    if (params.size() == 0)
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("PING")));
    server.messageToClient(client, client, "PONG :" + params[0]);
}