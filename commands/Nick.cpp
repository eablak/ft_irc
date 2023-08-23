#include "../includes/Nick.hpp"

Nick::Nick()
{
}

Nick::~Nick() {}

void Nick::execute(Server &server, Client *client)
{

    if (client->getParams().empty())
    {
        Numeric::printNumeric(client, server, ERR_NONICKNAMEGIVEN());
        return;
    }
    else
    {
        for (size_t i = 0; i < server.getClients().size(); i++)
        {
            if (server.getClients()[i]->getNickname() == client->getParams()[0])
            {
                Numeric::printNumeric(client, server, ERR_NICKNAMEINUSE(client->getParams()[0]));
                return;
            }
        }

        if ((client->getParams()[0][0] == ':' && client->getParams().size() == 1) || client->getParams().size() != 1)
        {
            std::cout << "size " << client->getParams().size() << std::endl;
            server.messageToClient(client->getClientFd(), "Error: Invalid nickname\n");
            return;
        }

            if(std::isdigit(client->getParams()[0][0])){

            std::cout << "first char is number\n" << std::endl;
            Numeric::printNumeric(client, server, ERR_ERRONEUSNICKNAME(client->getParams()[0]));
            return;
            }
        

        for (unsigned long i = 0; i < client->getParams()[0].size(); i++)
        {
            if (!std::isprint(client->getParams()[0][i]) && client->getParams()[0][i] != '\r' && client->getParams()[0][i] != '\n')
            {
                Numeric::printNumeric(client, server, ERR_ERRONEUSNICKNAME(client->getParams()[0]));
                return;
            }
        }

        if (client->getNickname().size() != 0)
        {
            std::string msg = client->getNickname() + " changed his nickname to " + client->getParams()[0] + "\n";
            server.messageToClient(client->getClientFd(), msg);
            client->setNickname(client->getParams()[0]);
            return;
        }

        client->setNickname(client->getParams()[0]);
        server.messageToClient(client->getClientFd(), "Requesting the new nick \"" + client->getNickname() + "\".\n");
    }
}
