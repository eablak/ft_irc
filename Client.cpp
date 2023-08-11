#include "includes/Client.hpp"

Client::Client(int _fd){
    fd = _fd;
    this->client_auth = NOTAUTHENTICATED;
}

int Client::getClientFd(){
    return (fd);
}