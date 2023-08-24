#include "../includes/Part.hpp"
Part::Part()
{
}

Part::~Part()
{
}

void Part::handleWithParams(Server &server, Client *client, std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("PART")));
        return;
    }
    if(!client->isInChannel(params[0]))
    {
        Numeric::printNumeric(client, server, ERR_NOTONCHANNEL(params[0]));
        return;
    }
    try
    {
        Channel &ch = server.getChannel(params[0]);
        ch.removeClient(client);
        client->removeChannel(ch);
        server.messageToClient(client, "You left " + params[0]);
    }
    catch(Server::ChannelNotFoundException &e)
    {
        Numeric::printNumeric(client, server, ERR_NOSUCHCHANNEL(params[0]));
    }
}
void Part::handleMultipleChannels(Server &server, Client *client, std::vector<std::string> &params)
{
    std::vector<std::string> channels = Utils::split(params[0], ',');
    std::vector<std::string>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<std::string> newParams;
        newParams.push_back(*it);
        handleWithParams(server, client, newParams);
    }
}


void Part::execute(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();
    if (params.size() == 0)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("PART"));
        return;
    }
    if (params[0].find(',') != std::string::npos)
    {
        handleMultipleChannels(server, client, params);
        return;
    }
    handleWithParams(server,client,params);
}