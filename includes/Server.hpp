#ifndef SERVER_HPP
#define SERVER_HPP

#include "librarys.hpp"
#include "Numeric.hpp"

class Client;

class Server{

    private:
    int port;
    std::string password;
    int socketfd;
    std::vector<pollfd> _pollfds;
    std::vector<Client > _clients;

    public:
    Server(std::string av1,std::string av2);

    void createSocket();
    void serverInvoke();
    void clientAccept();
    void messageToClient(int fd, std::string msg);
    void clientEvent(int fd);
    std::string readMessage(int fd);
    Client &getClient(int fd);
    void handleMsg(Client &client, std::string msg);
};


#endif