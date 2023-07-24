#ifndef SERVER_HPP
#define SERVER_HPP
#include "checks.hpp"
#include "Server.hpp"
#include <iostream>

using namespace std;

class Server{

    private:
    int port;
    string password;

    public:
    Server(string av1,string av2);

};

#endif