#include "../includes/Join.hpp"
#include "../includes/Numeric.hpp"
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
void Join::handleWithParams(Server &server, Client &client, std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("JOIN")));
        return;
    }
    checkChannelName(params[0]);
    std::string channelName = params[0];
    std::vector<Channel> &channels = server.getChannels();
    std::vector<Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->getName() == channelName)
        {
            if (it->isClientInChannel(client))
            {
                server.messageToClient(client.getClientFd(), "You are already in " + channelName + "\n");
                return;
            }
            it->addClient(client);
            sendSuccessNumerics(server, client, *it);
            return;
        }
    }
    Channel newChannel(channelName, client);
    channels.push_back(newChannel);
    sendSuccessNumerics(server, client, newChannel);
}

void Join::execute(Server &server, Client &client)
{
    std::vector<std::string> params = client.getParams();
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
    checkChannelName(params[0]);
    std::string channelName = params[0];
    std::vector<Channel> &channels = server.getChannels();
    std::vector<Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->getName() == channelName)
        {
            if (it->isClientInChannel(client))
            {
                server.messageToClient(client.getClientFd(), "You are already in " + channelName + "\n");
                return;
            }
            it->addClient(client);
            sendSuccessNumerics(server, client, *it);
            return;
        }
    }
    Channel newChannel(channelName, client);
    channels.push_back(newChannel);
    sendSuccessNumerics(server, client, newChannel);
}

void Join::sendSuccessNumerics(Server &server, Client &client, Channel &channel)
{
    server.messageToClient(client.getClientFd(), "You joined " + channel.getName() + "\n");
    Numeric::printNumeric(client, server, channel.getTopic().size() > 0 ? RPL_TOPIC(channel.getName(), channel.getTopic()) : RPL_NOTOPIC(channel.getName()));
    for (std::vector<Client>::iterator it = channel.getClients().begin(); it != channel.getClients().end(); it++){
    std::string printedName = it->getNickname().empty() ? it->getUsername() : it->getNickname();
        Numeric::printNumeric(client, server, RPL_NAMREPLY(channel.getName(), printedName));
    }
    Numeric::printNumeric(client, server, RPL_ENDOFNAMES(channel.getName()));
}

void Join::handleMultipleChannels(Server &server, Client &client, std::vector<std::string> &params)
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