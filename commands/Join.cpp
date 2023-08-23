#include "../includes/Join.hpp"
#include "../includes/Numeric.hpp"
#include <cstdio>
Join::~Join()
{
}

Join::Join()
{
}

void checkChannelName(std::string &channelName)
{
    const char invalidChars[] = {' ', ','};
    for (unsigned long int i = 0; i < sizeof(invalidChars); i++)
    {
        if (channelName.find(invalidChars[i]) != std::string::npos)
            throw std::invalid_argument("Invalid channel name");
    }
    if (channelName[0] != '#')
        throw std::invalid_argument("Invalid channel name");
}
void Join::handleWithParams(Server &server, Client *client, std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return;
    }
    checkChannelName(params[0]);
    std::string channelName = params[0];
    if (client->isInChannel(channelName))
    {
        server.messageToClient(client->getClientFd(), "You are already in this channel");
        return;
    }
    try
    {
        Channel &ch = server.getChannel(channelName);
        ch.addClient(client);
        client->addChannel(ch);
        sendSuccessNumerics(server, client, ch);
    }
    catch (Server::ChannelNotFoundException &e)
    {
        server.addChannel(channelName, client);
        Channel &newChannel = server.getChannel(channelName);
        client->addChannel(newChannel);

        sendSuccessNumerics(server, client, newChannel);
    }
}

void Join::execute(Server &server, Client *client)
{
    std::vector<std::string> params = client->getParams();
    if (params.size() != 1)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return;
    }
    if (params[0].find(',') != std::string::npos)
    {
        handleMultipleChannels(server, client, params);
        return;
    }
    handleWithParams(server, client, params);
}

void Join::sendSuccessNumerics(Server &server, Client *client, Channel &channel)
{

    server.messageToClient(client->getClientFd(), "You joined " + channel.getName());
    Numeric::printNumeric(client, server, channel.getTopic().size() > 0 ? RPL_TOPIC(channel.getName(), channel.getTopic()) : RPL_NOTOPIC(channel.getName()));
    std::string names;
    for (std::vector<Client *>::iterator it = channel.getClients().begin(); it != channel.getClients().end(); it++)
    {
        names += (*it)->getNickname().empty() ? (*it)->getUsername() : (*it)->getNickname();
        if (it != channel.getClients().end() - 1)
            names += " ";
    }
    Numeric::printNumeric(client, server, RPL_NAMREPLY(channel.getName(), names));
    Numeric::printNumeric(client, server, RPL_ENDOFNAMES(channel.getName()));
}

void Join::handleMultipleChannels(Server &server, Client *client, std::vector<std::string> &params)
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
