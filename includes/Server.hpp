#ifndef SERVER_HPP
#define SERVER_HPP

#include "libraries.hpp"
#include "Client.hpp"
#include "error.hpp"
#include "Numeric.hpp"
#include "checks.hpp"
#include "HandleMessage.hpp"

class HandleMessage;
class Client;
class Numeric;

class Server
{

private:
    int port;
    std::string password;
    int socketfd;
    std::vector<pollfd> _pollfds;
    std::vector<Client *> _clients;
    std::string hostname;
    std::vector<Channel> _channels;

public:
    Server(std::string av1, std::string av2);
    ~Server();
    void createSocket();
    void serverInvoke();
    void clientAccept();
    void messageToClient(Client *sender, Client *target, std::string msg);
    void clientEvent(int fd);
    std::string readMessage(int fd);
    Client *getClient(int fd);
    std::string getPassword();
    void setHostname();
    std::string getHostname();
    std::vector<Channel> &getChannels();
    std::vector<Client *> &getClients();
    void removeClient(Client *client);
    Channel &getChannel(std::string &channelName);
    void addChannel(std::string channelName, Client *client);
    void removeChannel(std::string channelName);
    Client *getClientByNickname(std::string nickname);
    class ClientDisconnectedException : public std::exception
    {
        virtual const char *what() const throw()
        {
            return "Client Disconnected";
        }
    };
    class ChannelNotFoundException : public std::exception
    {
        virtual const char *what() const throw()
        {
            return "Channel not found";
        }
    };
    class ClientNotFoundException : public std::exception
    {
        virtual const char *what() const throw()
        {
            return "Client not found";
        }
    };
};

#endif
