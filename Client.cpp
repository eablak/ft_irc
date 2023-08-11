#include "includes/Client.hpp"

Client::Client(int _fd){
    fd = _fd;
    this->client_auth = NOTAUTHENTICATED;
}

int Client::getClientFd(){
    return (fd);
}

void Client::setMsg(std::string msg){ //kullanmayabilirm
    current_msg = msg;
}

_auth Client::getAuthStatus(){
    return (client_auth);
}

void Client::setClientMessage(std::string command, std::string args){
    client_message[command] = args;
}