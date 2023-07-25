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


using namespace std;

class Server{

    private:
    int port;
    string password;
    int socketfd;

    public:
    Server(string av1,string av2);
    void createSocket();
};

#endif