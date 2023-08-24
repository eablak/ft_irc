#include "../includes/Pass.hpp"

Pass::Pass()
{
}

Pass::~Pass() {}

void Pass::execute(Server &server, Client *client)
{

    if (client->getAuthStatus() != NOTAUTHENTICATED)
    {
        Numeric::printNumeric(client, server, ERR_ALREADYREGISTRED());
        return;
    }
    if (client->getParams().empty())
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(client->getCommand()));
        return;
    }

    client->setParamsEnd();
    if (client->getParams()[0] == server.getPassword())
    {
        client->setAuthStatus(AUTHENTICATE);
        server.messageToClient(client->getClientFd(),"Password is correct!");
    }
    else
        Numeric::printNumeric(client, server, ERR_PASSWDMISMATCH());
    
}
