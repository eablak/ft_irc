#include "../includes/Client.hpp"

Client::Client(int _fd){
    fd = _fd;
}

int Client::checkPass(string _pass){
    std::cout << _pass << endl;
    return 1;
}