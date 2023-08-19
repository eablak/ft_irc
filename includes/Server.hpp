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
    std::vector<Client> _clients;
    std::string hostname;
    std::vector<Channel> _channels;

public:
    Server(std::string av1, std::string av2);
    void createSocket();
    void serverInvoke();
    void clientAccept();
    void messageToClient(int fd, std::string msg);
    void clientEvent(int fd);
    std::string readMessage(int fd);
    Client &getClient(int fd);
    std::string getPassword();
    void setHostname();
    std::string getHostname();
    std::vector<Channel> &getChannels();
    std::vector<Client> &getClients();
};

#endif
