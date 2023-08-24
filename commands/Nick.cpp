#include "../includes/Nick.hpp"

Nick::Nick()
{
}

Nick::~Nick() {}

void Nick::execute(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();

    if (params.size() < 1)
    {
        Numeric::printNumeric(client, server, ERR_NONICKNAMEGIVEN());
        return;
    }
    for (size_t i = 0; i < server.getClients().size(); i++)
    {
        if (server.getClients()[i]->getNickname() == params[0])
        {
            Numeric::printNumeric(client, server, ERR_NICKNAMEINUSE(params[0]));
            return;
        }
    }
    if (params[0][0] == ':' || std::isdigit(params[0][0]))
    {
        server.messageToClient(client, client, ERR_ERRONEUSNICKNAME(params[0]));
        return;
    }
    for (unsigned long i = 0; i < params[0].size(); i++)
    {
        if (!std::isprint(params[0][i]) && params[0][i] != '\r' && params[0][i] != '\n')
        {
            Numeric::printNumeric(client, server, ERR_ERRONEUSNICKNAME(params[0]));
            return;
        }
    }
    if (client->getNickname().size() != 0)
    {
        std::string msg = client->getNickname() + " NICK changed his nickname to " + params[0];
        server.messageToClient(client, client, msg);
        client->setNickname(params[0]);
        return;
    }

    client->setNickname(params[0]);
    server.messageToClient(client, client, "NICK Requesting the new nick " + client->getNickname());
    if (client->getUsername().size() == 0 || client->getRealname().size() == 0)
        return;
    client->setAuthStatus(REGISTERED);
    Numeric::printNumeric(client, server, RPL_WELCOME(client->getNickname(), client->getUsername(), server.getHostname()));
}
