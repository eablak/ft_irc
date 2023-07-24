#ifndef SERVER_HPP
#define SERVER_HPP
#include "checks.hpp"
#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

class Server{

    private:
    int port;
    string password;
    // int serverSocket;
    // int newSocket;

    public:
    Server(string av1,string av2);
    void server_args(string port, string password);
};


#endif