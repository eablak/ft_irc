#include "includes/Server.hpp"
#include <cstdlib>
#include <string>

Server::Server(string _port, string _password){

    this->port = std::stoi(_port);
    this->password = _password;
    createSocket();
}

void Server::createSocket(){
    int serverSocket;
    struct sockaddr_in serverAddr;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) 
        error::error_func("Error creating socket");
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        error::error_func("Error while setting socket options");
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
        error::error_func("Error while setting socket flag options");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        error::error_func("Error binding socket.");

    listen(serverSocket, 100);

    std::cout << "Server listening on port " << port  <<  "..." << std::endl;

    close(serverSocket);
}