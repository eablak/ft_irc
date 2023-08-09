#include "../includes/Client.hpp"

Client::Client(int _fd){
    fd = _fd;
}

int Client::checkPass(string _pass){
    std::cout << _pass << endl;
    return 1;
}

int Client::getClientFd(){
    return (fd);
}

string Client::getClientMsg(Client *client){
    std::string clientMsg;
    char buffer[1024];
    int clientFd = client->getClientFd();
    int ret = recv(clientFd, buffer, 1024 ,0);
    if (ret == -1){
        cout << "Failed to recv" << endl;
        exit(1);
    }
    if (ret == 0)
        throw ClientRecvException();
    clientMsg = buffer;
    return (clientMsg);
}
