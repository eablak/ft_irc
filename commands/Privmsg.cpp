#include "../includes/Privmsg.hpp"

Privmsg::Privmsg()
{
}

Privmsg::~Privmsg()
{
}
void Privmsg::sendMessageToClient(Server &server, Client &client, std::vector<std::string> params)
{
    try
    {
        Client &target = server.getClientByNickname(params[0]);
        std::string message = ":" + client.getNickname() + " PRIVMSG " + params[0] + " :" + params[1] + "\r\n";
        server.messageToClient(target.getClientFd(), message);
    }
    catch (Server::ClientNotFoundException &e)
    {
        Numeric::printNumeric(client, server, ERR_NOSUCHNICK(params[0]));
        return;
    }
}
void Privmsg::sendMessageToChannel(Server &server, Client &client, std::vector<std::string> params)
{
    try
    {
        Channel &ch = server.getChannel(params[0]);
        if (!ch.isClientInChannel(client))
        {
            Numeric::printNumeric(client, server, ERR_NOTONCHANNEL(params[0]));
            return;
        }
        std::string message = ":" + client.getNickname() + " PRIVMSG " + params[0] + " :" + params[1] + "\r\n";
        ch.sendMessageToChannel(server, client, message);
    }
    catch (Server::ChannelNotFoundException &e)
    {
        Numeric::printNumeric(client, server, ERR_NOSUCHCHANNEL(params[0]));
        return;
    }
}

void Privmsg::handleWithParams(Server &server, Client &client, std::vector<std::string> params)
{
    if (params[0][0] == '#')
        sendMessageToChannel(server, client, params);

    else
        sendMessageToClient(server, client, params);
    
}
void Privmsg::handleMultipleTargets(Server &server, Client &client, std::vector<std::string> params)
{
    std::vector<std::string> targets = Utils::split(params[0], ',');
    std::vector<std::string>::iterator it;
    for (it = targets.begin(); it != targets.end(); it++)
    {
        std::vector<std::string> newParams;
        newParams.push_back(*it);
        newParams.push_back(params[1]);
        handleWithParams(server, client, newParams);
    }
}
std::vector<std::string> Privmsg::fixParams(std::vector<std::string> &params)
{
    std::vector<std::string> newParams;
    newParams.push_back(params[0]);
    for(size_t i = 1 ;i < params.size(); i++)
    {
        if(params[i][0] == ':')
        {
            std::string tmp = params[i];
            tmp.erase(0,1);
            for(size_t j = i + 1; j < params.size(); j++)
            {
                tmp += " " + params[j];
            }
            newParams.push_back(tmp);
            break;
        }
    }
    return newParams;
}
void Privmsg::execute(Server &server, Client &client)
{
    std::vector<std::string> params = client.getParams();
    params = fixParams(params);
    for (size_t i = 0; i < params.size(); i++)
    {
        std::cout << "params[" << i << "] = " << params[i] << std::endl;
    }
    if (params.size() < 2)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("PRIVMSG"));
        return;
    }
    if (params[0].find(',') != std::string::npos)
    {
        handleMultipleTargets(server, client, params);
        return;
    }
    handleWithParams(server, client, params);
}