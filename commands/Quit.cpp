#include "../includes/Quit.hpp"

Quit::Quit()
{
}

Quit::~Quit()
{
}

    std::vector<std::string> params = Utils::concatParams(client->getParams());
    if (params.size() != 1 && params.size() != 0)
        return; // değişcek
    if (params.empty())
        server.removeClient(client);
    else
    {
        std::vector<Client *> clients = server.getClients();
        for (size_t i = 0; i < clients.size(); i++)
            server.messageToClient(clients[i]->getClientFd(), client->getNickname() + " " + "QUIT " + params[0]);
        server.removeClient(client);
    }
}