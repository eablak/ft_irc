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
    if (new_params.size() != 4 || (new_params[1] != "0"))
    {
        server.messageToClient(client, client, "Error: Missing parameter");
        return;
    }

    client->setUsername(new_params[0]);
    client->setRealname(new_params[3]);
    if (client->getNickname().size() == 0)
        return;
    client->setAuthStatus(REGISTERED);
    new_params.clear();
}
