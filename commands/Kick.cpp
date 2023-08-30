#include "../includes/Kick.hpp"

Kick::Kick()
{
}

Kick::~Kick()
{
}

void Kick::execute(Server &server, Client *client)
{
    std::vector<std::string> params = Utils::concatParams(client->getParams());
    if (params.size() < 2)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("KICK"));
        return;
    }
    std::string channelName = params[0];
    std::string nickname = params[1];
    std::string reason = "";
    if (params.size() > 2)
        reason = params[2];

    try
    {
        Channel &channel = server.getChannel(channelName);
        Client *clientToKick = server.getClientByNickname(nickname);
        if (!channel.isClientInChannel(clientToKick))
        {
            Numeric::printNumeric(client, server, ERR_USERNOTINCHANNEL(nickname, channelName));
            return;
        }
        if (!channel.isClientOperator(client))
        {
            Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(channelName));
            return;
        }
        channel.sendMessageToChannel(server, client, "KICK " + channelName + " " + clientToKick->getNickname() + " :" + reason);
        channel.removeClient(clientToKick);
        clientToKick->removeChannel(channel);
    }
    catch (std::exception &e)
    {
        if (typeid(e) == typeid(Server::ChannelNotFoundException))
            Numeric::printNumeric(client, server, ERR_NOSUCHCHANNEL(channelName));
        else if (typeid(e) == typeid(Server::ClientNotFoundException))
            Numeric::printNumeric(client, server, ERR_NOSUCHNICK(nickname));
        return;
    }
}
