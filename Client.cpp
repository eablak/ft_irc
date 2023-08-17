#include "includes/Client.hpp"

Client::Client(){
}

Client::Client(int _fd){
    fd = _fd;
    this->client_auth = NOTAUTHENTICATED;
}

int Client::getClientFd(){
    return (fd);
}

_auth Client::getAuthStatus(){
    return (client_auth);
}

void Client::setClientMessage(std::string command, std::string args){

    if (this->client_message.size() == 1){
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

void Client::setParamsEnd(){
    if (!params.empty()){
        std::string &str = params.back();
        if (str[str.size()- 1] == '\r'){
            str.erase(str.size()- 1);
        }
    }
}

Numeric &Client::getNums(){
    return (_nums);
}

std::string Client::getNickname(){
    return (nickname);
}

std::string Client::getUsername(){
    return (username);
}

void Client::setNickname(std::string _nickname){
    nickname = _nickname;
}

void Client::setUsername(std::string _username){
    username = _username;
}

std::string Client::getCommand(){
    return command;
}

std::vector<std::string> &Client::getParams(){
    return params;
}

void Client::setParams(std::vector<std::string> tmp){
    for(size_t i = 0; i< tmp.size(); i++){
        params.push_back(tmp[i]);
    }
}

void Client::setCommand(std::string _command){
    command = _command;
}

