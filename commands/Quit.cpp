#include "../includes/Quit.hpp"
#include <ctime>

Quit::Quit()
{
}

Quit::~Quit()
{
}
void checkLastOperator(Server &server, Client *client, Channel &ch)
{
    int is_op = 0;
    int any_other_op = 0;
    if (ch.isClientOperator(client))
        is_op = 1;
    if (is_op == 1 && ch.getOperators().size() > 1)
        any_other_op = 1;
    ch.removeClient(client);
    Client *new_op;
    if (is_op == 1 && any_other_op == 0 && ch.getClients().size() != 0)
    {
        std::srand(std::time(0));
        size_t rand_op = std::rand() % ch.getClients().size();

        new_op = ch.getClients()[rand_op];
        ch.addOperator(new_op);
        ch.sendMessageToChannel(server, new_op, "MODE " + ch.getName() + " +o " + new_op->getNickname());
    }
}
void partFromAllChannels(Server &server, Client *client)
{
    std::vector<Channel> &channels = server.getChannels();
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].isClientInChannel(client))
        {
            channels[i].sendMessageToChannel(server, client, "PART " + channels[i].getName() + " :");
            checkLastOperator(server, client, channels[i]);
            client->removeChannel(channels[i]);
            if (channels[i].getClients().size() == 0)
                server.removeChannel(channels[i]);
        }
    }
}
void Quit::execute(Server &server, Client *client)
{
    std::vector<std::string> params = Utils::concatParams(client->getParams());
    if (params.size() < 1)
    {
        server.messageToClient(client, client, "QUIT :");
        partFromAllChannels(server, client);
        
        server.removeClient(client);
        return;
    }
    else
    {
        std::vector<Client *> clients = server.getClients();
        for (size_t i = 0; i < clients.size(); i++)
            server.messageToClient(client, clients[i], client->getNickname() + " " + "QUIT " + params[0]);
        partFromAllChannels(server, client);

        server.removeClient(client);
    }
}
