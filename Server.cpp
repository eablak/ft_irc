#include "includes/Server.hpp"

Server::Server(string port, string _password){
    try{
        int _port = stoi(port);
        if (_port < 1024 || _port > 65535){
            cerr << "wrong port range "<< endl;
            exit(1);
        }
        this->port = _port;
        this->password = _password;
    } catch(const exception &e){
        cerr << "wrong port" << endl;
    }
}