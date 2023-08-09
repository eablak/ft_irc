#include "../includes/Client.hpp"
#include "../includes/Numeric.hpp"

Client::Client(int _fd){
    fd = _fd;
    auth = false;
    map_auth->insert(pair<string,bool>("PASS",false));
    map_auth->insert(pair<string,bool>("USER",false));
    map_auth->insert(pair<string,bool>("NICK",false));
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

bool Client::getAuth() {
    return (auth);
}

void Client::NotAuthClient(Client *client, Server *server, string msg){
    cout << msg << " fd: "<< client->fd << " auth değil" <<endl;
    // msg trimle ona göre noauthexec yaz;
    // if send numeric else execnotauth
    string command;
    if (msg.find(" ") == std::string::npos)
		msg = "";
	else
		msg = msg.substr(msg.find(" ") + 1);
    if (msg != "PASS")
        numeric::sendNumeric(ERR_NOTREGISTERED,client->fd);
    (void) server;
    return;
}   