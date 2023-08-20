#include "../includes/User.hpp"

User::User()
{
}

User::~User() {}

void User::execute(Server &server, Client &client)
{
    if (client.getAuthStatus() == REGISTERED)
    {
        Numeric::printNumeric(client, server, ERR_ALREADYREGISTRED());
        return;
    }
    if (client.getParams().empty())
    {
        Numeric::printNumeric(client, server, ERR_NONICKNAMEGIVEN());

        return;
    }
    if (client.getParams().size() != 4)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("USER"));
        return;
    }
    std::vector<std::string> new_params;
    std::string all;
    for (size_t i = 0; i < client.getParams().size(); i++)
    {
        if (client.getParams()[i][0] == ':')
        {
            while (i < client.getParams().size())
            {
                all += client.getParams()[i];
                if (i != client.getParams().size() - 1)
                    all += " ";
                i++;
            }
            new_params.push_back(all);
        }
        else
            new_params.push_back(client.getParams()[i]);
    }
    if (new_params.size() != 4 || (new_params[1] != "0" || new_params[2] != "*"))
    {
        server.messageToClient(client.getClientFd(), "Error: Missing parameter\n");
        return;
    }

    client.setUsername(new_params[0]);
    client.setRealname(new_params[3]);
    client.setAuthStatus(REGISTERED);
    server.messageToClient(client.getClientFd(),"001 test \r\n");
    Numeric::printNumeric(client, server, RPL_WELCOME(client.getNickname(), client.getUsername(), server.getHostname()));
    Numeric::printNumeric(client, server, RPL_YOURHOST(client.getNickname(), server.getHostname()));
    Numeric::printNumeric(client, server, RPL_CREATED(client.getNickname(), std::string("tempDate")));

    new_params.clear();
}
