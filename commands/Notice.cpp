#include "../includes/Notice.hpp"

Notice::Notice(){};
Notice::~Notice(){};

void Notice::channelOrUser(Server &server, Client *client, std::vector<std::string> params)
{
    if (params[0][0] == '#')
        sendToMessageChannel(server, client, params);
    else
        sendToMessageUser(server, client, params);
}

void Notice::sendToMessageChannel(Server &server, Client *client, std::vector<std::string> params)
{
    try
    {
        Channel &ch = server.getChannel(params[0]);
        if (!ch.isClientInChannel(client))
            return;
        std::string message = "NOTICE " + params[0] + " :" + params[1] + "\r\n";
        ch.sendMessageToChannel(server, client, message);
    }
    catch(Server::ChannelNotFoundException &e)
    {
        return;
    }
}

void Notice::sendToMessageUser(Server &server, Client *client, std::vector<std::string> params)
{
    try
    {
        Client *target = server.getClientByNickname(params[0]);
        std::string message = " NOTICE " + params[0] + " :" + params[1];
        server.messageToClient(client, target, message);
    }
    catch(const std::exception& e)
    {
        return;
    }
}

void Notice::multipleTargets(Server &server, Client *client, std::vector<std::string> params)
{
    std::vector<std::string> targets = Utils::split(params[0], ',');
    std::vector<std::string>::iterator it;
    for (it = targets.begin(); it != targets.end(); it++)
    {
        std::vector<std::string> newParams;
        newParams.push_back(*it);
        newParams.push_back(params[1]);
        channelOrUser(server, client, newParams);
    }
}

void Notice::execute(Server &server, Client *client)
{
    std::vector<std::string> params = Utils::concatParams(client->getParams());
    params = Utils::concatParams(params);
    if (params.size() < 2)
        return;
    if (params[0].find(',') != std::string::npos)
    {
        multipleTargets(server,client,params);
        return;
    }
    channelOrUser(server, client, params);
}