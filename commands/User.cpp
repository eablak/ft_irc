#include "../includes/User.hpp"

User::User()
{
}

User::~User() {}

void User::execute(Server &server, Client *client)
{
    if (client->getAuthStatus() == REGISTERED)
    {
        Numeric::printNumeric(client, server, ERR_ALREADYREGISTRED());
        return;
    }
    if (client->getParams().empty())
    {
        Numeric::printNumeric(client, server, ERR_NONICKNAMEGIVEN());

        return;
    }
    if (client->getParams().size() < 4)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("USER"));
        return;
    }
    std::vector<std::string> new_params = Utils::concatParams(client->getParams());
    // * yerine hostname gelebiliyor silindi
    if (new_params.size() != 4 || (new_params[1] != "0"))
    {
        server.messageToClient(client->getClientFd(), "Error: Missing parameter");
        return;
    }

    client->setUsername(new_params[0]);
    client->setRealname(new_params[3]);
    client->setAuthStatus(REGISTERED);
    Numeric::printNumeric(client, server, client->getPrefix(server) + " " +RPL_WELCOME(client->getNickname(), client->getUsername(), server.getHostname()));
    new_params.clear();
}
