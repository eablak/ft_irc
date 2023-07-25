#include "includes/Server.hpp"
#include <cstdlib>
#include <string>

Server::Server(string _port, string _password){

    this->port = std::stoi(_port);
    this->password = _password;
    createSocket();
}

void Server::createSocket(){
    int opt = 1;
    struct sockaddr_in serverAddr;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        error::error_func("Error socket failed");
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        error::error_func("Error setsocket options");
    if (fcntl(socketfd, F_SETFL, O_NONBLOCK) < 0)
        error::error_func("Error while setting socket flag options");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        error::error_func("Error binding socket.");
    if (listen(socketfd, 100) < 0)
        error::error_func("Error listen socket");
}