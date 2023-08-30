#include "includes/Client.hpp"
#include "includes/Server.hpp"
Client::Client()
{
}

Client::Client(int _fd, std::string _hostname)
{
    fd = _fd;
    this->client_auth = NOTAUTHENTICATED;
    this->hostname = _hostname;
}

int Client::getClientFd()
{
    return (fd);
}

_auth Client::getAuthStatus()
{
    return (client_auth);
}

void Client::setAuthStatus(_auth status)
{
    this->client_auth = status;
}

void Client::setParamsEnd()
{
    if (!params.empty())
    {
        std::string &str = params.back();
        if (str[str.size() - 1] == '\r')
        {
            str.erase(str.size() - 1);
        }
    }
}

std::string Client::getNickname()
{
    return (nickname);
}

std::string Client::getUsername()
{
    return (username);
}

void Client::setNickname(std::string _nickname)
{
    nickname = _nickname;
}

void Client::setUsername(std::string _username)
{
    username = _username;
}

std::string Client::getCommand()
{
    return command;
}

std::vector<std::string> &Client::getParams()
{
    return params;
}

void Client::setParams(std::vector<std::string> tmp)
{
    params.clear();
    params = tmp;
}

void Client::setCommand(std::string _command)
{
    command = _command;
}

void Client::setRealname(std::string _realname)
{
    realname = _realname;
}

bool Client::isInChannel(std::string &channelName) const
{
    for (size_t i = 0; i < joinedChannels.size(); i++)
    {
        if (joinedChannels[i].getName() == channelName)
            return true;
    }
    return false;
}

void Client::addChannel(Channel &channel)
{
    joinedChannels.push_back(channel);
}

void Client::removeChannel(Channel &channel)
{
    std::vector<Channel>::iterator it;
    for (it = joinedChannels.begin(); it != joinedChannels.end(); it++)
    {
        if (it->getName() == channel.getName())
        {
            joinedChannels.erase(it);
            return;
        }
    }
}

std::string Client::getPrefix()
{
    return (":" + nickname + "!" + username + "@" + this->hostname + ":");
}

std::string Client::getRealname()
{
    return (realname);
}

std::vector<Channel> &Client::getChannels()
{
    return (joinedChannels);
}
void Client::setMessage(std::string message)
{
    this->message = message;
}

std::string Client::getMessage()
{
    return (this->message);
}

