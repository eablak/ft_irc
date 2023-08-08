#include "../includes/Server.hpp"
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
    if (::bind(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        error::error_func("Error binding socket.");
    if (listen(socketfd, 100) < 0)
        error::error_func("Error listen socket");
    cout << "Server listening "<< port << " port.." << endl;
}

void Server::clientAccept(){
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(socketfd,(sockaddr *)&client_addr,&len);
    if (client_fd == -1)
        error::error_func("Error accept");
    else{
        pollfd poll_client;
        poll_client.fd = client_fd;
        poll_client.events = POLLIN;
        _pollfds.push_back(poll_client);
        Client *my_client = new Client(client_fd);
        std::cout << "fd "<< client_fd << " client succesfully connected\n";
        _clients.push_back(my_client);
        messageToClient(my_client, "Enter Password\n");
    }        
}

void Server::clientRevent(){
    std::cout << "revent " << std::endl;
}


void Server::serverInvoke(){
    pollfd initalize = {socketfd,POLLIN,0};
    _pollfds.push_back(initalize);
    while(1){
        if (poll(&_pollfds[0],_pollfds.size(),0) == -1)
            error::error_func("Error while polling");
        for(size_t i = 0; i < _pollfds.size(); i ++){
            if (_pollfds[0].revents == 0)
                continue;
            if ((_pollfds[0].revents & POLLIN) && _pollfds[i].fd == socketfd)
                clientAccept();
            if ((_pollfds[0].revents & POLLIN) && _pollfds[i].fd != socketfd)
                clientRevent();
        }
    }
}

Server::~Server(){
    close(socketfd);
}



void Server::messageToClient(Client *client, std::string msg){
    int clientfd = client->getClientFd(client);
    if (send(clientfd,msg.c_str(),msg.length(),0) < 0)
    {
        std::cerr << "Failed to send message" << std::endl;
		exit(1);
    }
}