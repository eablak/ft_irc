#include "../includes/Quit.hpp"

Quit::Quit()
{
}

Quit::~Quit()
{
}

void Quit::execute(Server &server, Client &client)
{
    std::vector<std::string> params = client.getParams();
    if (params.empty())
        server.removeClient(client);
    else
        server.removeClient(client);
}