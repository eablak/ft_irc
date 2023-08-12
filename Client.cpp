#include "includes/Client.hpp"

Client::Client(){
}

Client::Client(int _fd){
    isim = "Tek ben";
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

    if (this->client_message.size() == 1){
        std::cout << "size 1" << std::endl;
        this->client_message.erase(this->client_message.begin());
    }
    this->client_message.push_back(std::make_pair(command,args));
}

std::list<std::pair<std::string, std::string> > Client::getMap(){
    return (client_message);
}


void Client::setAuthStatus(_auth status){
    this->client_auth = status;
}

void Client::printMap(){
    std::cout << "map size : " << client_message.size() << std::endl;
    std::cout << "ilk değer: " << client_message.begin()->first << std::endl;
    std::cout << "ikinci değer: " << client_message.begin()->second << std::endl;
}

std::string Client::getISIM()
{
    return (this->isim);
}