#include "../includes/Names.hpp"

Names::Names()
{
}

Names::~Names()
{
}
void Names::handleMultipleChannels(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();
    std::vector<std::string> channels = Utils::split(params[0], ',');
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        std::string channelName = *it;
        std::vector<std::string> params;
        params.push_back(channelName);
        handleWithParams(server, client, params);
    }
}

void Names::handleWithParams(Server &server, Client *client, std::vector<std::string> &params)
{
    std::string channelName = params[0];
    try
    {
        Channel &channel = server.getChannel(channelName);
        std::vector<Client *> &clients = channel.getClients();
        for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            Numeric::printNumeric(client, server, RPL_NAMREPLY(channelName, (*it)->getNickname()));
        }
        Numeric::printNumeric(client, server, RPL_ENDOFNAMES(channelName));
    }
    catch (Server::ChannelNotFoundException &e)
    {
        Numeric::printNumeric(client, server, ERR_NOSUCHCHANNEL(channelName));
        return;
    }
}
void Names::execute(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();
    if (params.size() == 0)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("NAMES"));
        return;
    }
    if (params[0].find(',') != std::string::npos)
    {
        handleMultipleChannels(server, client);
        return;
    }
    handleWithParams(server, client, params);
}