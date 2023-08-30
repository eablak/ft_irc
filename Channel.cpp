#include "includes/Server.hpp"
#include "includes/Channel.hpp"
#include "includes/Numeric.hpp"
Channel::Channel(std::string name, Client *client)
{
    this->name = name;
    this->topic = "";
    this->operators.push_back(client);
    this->clients.push_back(client);
}

Channel::~Channel()
{
}

std::string Channel::getName() const
{
    return (this->name);
}

std::string Channel::getTopic() const
{
    return (this->topic);
}

std::vector<Client *> &Channel::getClients()
{
    return (this->clients);
}

std::vector<Client *> &Channel::getOperators()
{
    return (this->operators);
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::addClient(Client *client)
{
    this->clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    std::vector<Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it)->getClientFd() == client->getClientFd())
        {
            this->clients.erase(it);
            return;
        }
    }
}

bool Channel::isClientInChannel(Client *client)
{
    std::vector<Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if ((*it)->getClientFd() == client->getClientFd())
            return (true);
    }
    return (false);
}

bool Channel::isClientOperator(Client *client)
{
    std::vector<Client *>::iterator it;
    for (it = this->operators.begin(); it != this->operators.end(); it++)
    {
        if ((*it)->getClientFd() == client->getClientFd())
            return (true);
    }
    return (false);
}
// sends a the message to AlL of the channel members including the client
void Channel::sendMessageToChannel(Server &server, Client *client, std::string message)
{
    std::vector<Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        server.messageToClient(client, *it, message);
    }
}
