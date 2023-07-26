#ifndef SERVER_HPP
#define SERVER_HPP
#include "checks.hpp"
#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include "error.hpp"
#include <vector>
#include <poll.h>
#include "Client.hpp"

using namespace std;

class Client;

class Server{

    private:
    int port;
    string password;
    int socketfd;
    vector<pollfd> _pollfd;
    vector<Client *> _clients;

    public:
    Server(string av1,string av2);
    void createSocket();
    void serverInvoke();
    void clientAccept();
};

#endif