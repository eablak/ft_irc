#include "../includes/Who.hpp"

Who::Who()
{
}

Who::~Who()
{
}

void Who::execute(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();
    if (params.size() < 1)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("WHO"));
        return;
    }
    std::string mask = params[0];
    try
    {
        Channel &ch = server.getChannel(mask);
        std::vector<Client *> clients = ch.getClients();
        for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            Client *c = *it;
            std::string nick = c->getNickname();
            std::string user = c->getUsername();
            std::string realname = c->getRealname();
            std::string status = "*";
            std::string hopcount = "0";
            std::string channel = ch.getName();
            std::string hostname = server.getHostname();
            Numeric::printNumeric(client, server, RPL_WHOREPLY(nick, channel, user, hostname, "localhost", nick, status, hopcount, realname));
            Numeric::printNumeric(client, server, RPL_ENDOFWHO(nick, channel));
        }
        return;
    }
    catch (Server::ChannelNotFoundException &e)
    {
    }
    try
    {
        Client *client = server.getClientByNickname(mask);
        Numeric::printNumeric(client, server, RPL_WHOREPLY(client->getNickname(), "*", client->getUsername(), server.getHostname(), "localhost", client->getNickname(), "*", "0", client->getRealname()));
        return;
    }
    catch (Server::ClientNotFoundException &e)
    {
        Numeric::printNumeric(client, server, ERR_NOSUCHNICK(mask));
        return;
    }
}