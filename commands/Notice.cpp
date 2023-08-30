#include "../includes/Notice.hpp"

Notice::Notice(){};
Notice::~Notice(){};

void Notice::ChannelOrUser(Server &server, Client *client, std::vector<std::string> params)
{
    if (params[0][0] == '#')
        sendToMessageChannel(server, client, params);
    else
        sendToMessageUser(server, client, params);
}

void Notice::sendToMessageChannel(Server &server, Client *client, std::vector<std::string> params)
{
    Channel &ch = server.getChannel(params[0]);
    if (!ch.isClientInChannel(client))
    {
        Numeric::printNumeric(client, server, ERR_NOTONCHANNEL(params[0]));
        return;
    }
    std::string message = "NOTICE " + params[0] + " :" + params[1] + "\r\n";
    ch.sendMessageToChannel(server, client, message);
}

void Notice::sendToMessageUser(Server &server, Client *client, std::vector<std::string> params)
{
    Client *target = server.getClientByNickname(params[0]);
    std::string message = " NOTICE " + params[0] + " :" + params[1];
    server.messageToClient(client, target, message);
}

void Notice::execute(Server &server, Client *client)
{
    std::vector<std::string> params = Utils::concatParams(client->getParams());
    params = Utils::concatParams(params);
    if (params.size() < 2)
    {
        // hata mesajı
        return;
    }
    if (params[0].find(',') != std::string::npos)
    {
        // 1 den fazla kişiye gönderilecek durumlarda bura
        return;
    }
    ChannelOrUser(server, client, params);
}