#include "../includes/Topic.hpp"
Topic::Topic()
{
}
Topic::~Topic()
{
}
void Topic::sendTopicToChannel(Server &server, Channel &channel)
{
    for (std::vector<Client *>::iterator it = channel.getClients().begin(); it != channel.getClients().end(); it++)
        Numeric::printNumeric(*it, server, RPL_TOPIC((*it)->getNickname(), channel.getName(), channel.getTopic()));
}
void Topic::execute(Server &server, Client *client)
{

    std::vector<std::string> params = Utils::concatParams(client->getParams());
    if (params.size() != 1 && params.size() != 2)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }
    if (client->isInChannel(params[0]))
    {
        Channel &ch = server.getChannel(params[0]);
        if (params.size() == 1)
        {
            if (ch.getTopic() != "")
                Numeric::printNumeric(client, server, RPL_TOPIC(client->getNickname(), params[0], ch.getTopic()));
            else
                Numeric::printNumeric(client, server, RPL_NOTOPIC(client->getNickname(), params[0]));
        }
        else if (params.size() == 2 && ch.isClientOperator(client))
        {
            if (ch.getTopic() != "")
                ch.setTopic(params[1]);
            else
                ch.setTopic(params[1]);
            sendTopicToChannel(server, ch);
        }
        else if (ch.isClientOperator(client) == 0)
        {
            Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(std::string("TOPIC")));
        }
        else
            Numeric::printNumeric(client, server, ERR_NOTONCHANNEL(params[0]));
    }
}
