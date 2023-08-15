#ifndef SERVER_HPP
#define SERVER_HPP

#include "librarys.hpp"
#include "Client.hpp"
#include "error.hpp"
#include "Numeric.hpp"
#include "checks.hpp"
#include "handleMessage.hpp" 

class handleMessage;
class Client;
class Numeric;
class handleMessage;

class Server{

    private:
    int port;
    std::string password;
    int socketfd;
    std::vector<pollfd> _pollfds;
    std::vector<Client > _clients;
    std::string hostname;

    public:
    Server(std::string av1,std::string av2);

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
};


#endif