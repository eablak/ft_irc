#include "includes/Server.hpp"

Server::Server(std::string _port, std::string _password){

    this->port = std::stoi(_port);
    this->password = _password;
    createSocket();
}

void Server::createSocket(){
    int opt = 1;
    struct sockaddr_in serverAddr;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // soket oluşturma -> SOCK_STREAM (TCP)
        error::error_func("Error socket failed");
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) //soket seçeneklerini ayarlama -> SO_REUSEADDR aynı adresi yeniden kullanma
        error::error_func("Error setsocket options");
    if (fcntl(socketfd, F_SETFL, O_NONBLOCK) < 0) // Soketi non-blocking moda ayarlamak için fcntl
        error::error_func("Error while setting socket flag options");

    serverAddr.sin_family = AF_INET; // sin_family ağ ailesini (IPv4) belirtir,
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (::bind(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) // bağlama
        error::error_func("Error binding socket.");
    if (listen(socketfd, 100) < 0) // bağlantı isteklerini dinler
        error::error_func("Error listen socket");
    std::cout << "Server listening "<< port << " port.." << std::endl;
}

void Server::messageToClient(int fd, std::string msg){
    if (send(fd,msg.c_str(),msg.size(),0) < 0) // msg.c_str() -> char * çevir yoksa hatalı değer gonderir
        error::error_func("Send Error");
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
        _clients.push_back(Client(client_fd)); 
        std::cout << "fd "<< client_fd << " client succesfully connected\n";
        messageToClient(client_fd, "Welcome to IRC. Please Enter Password\n");
    }        
}

Client Server::getClient(int fd){
    for(size_t i = 0; i < _clients.size(); i++){
        if (_clients[i].getClientFd() == fd)
            return (_clients[i]);
    }
    std::cout << "getClient Error" << std::endl;
    exit(1);
}

std::string Server::readMessage(int fd){
    std::vector<char> buffer(5000);
    int bytesRead = recv(fd, buffer.data(), buffer.size(), 0);
    if (bytesRead == -1) {
        std::cerr << "Receive error." << std::endl;
        return (NULL);
    }
    if (bytesRead == 0)
    {
        std::cout << "fd " << fd << " disconnect" << std::endl;
        close(fd);    
    }
    return (buffer.data());
}

void Server::clientEvent(int fd){
    Client client = getClient(fd);
    std::string  msg = readMessage(client.getClientFd());
    client.setMsg(msg);
    std::cout << "fd " << fd << " " << msg;
}

void Server::serverInvoke(){
    pollfd initalize = {socketfd,POLLIN,0}; // soketin giriş olaylarını (POLLIN) izlemek üzere 
    _pollfds.push_back(initalize);
    while(1){
        if (poll(&_pollfds[0],_pollfds.size(),0) == -1) // poll gelen verileri beklemek ve bunları işlemek için
            error::error_func("Error while polling");
        for(size_t i = 0; i < _pollfds.size(); i ++){
            if (_pollfds[i].revents == 0) // olay yoksa devam
                continue;
            if (_pollfds[i].revents & POLLIN){ // giriş varsa
                if (_pollfds[i].fd == socketfd) // bağlantı
                    clientAccept();
                else
                    clientEvent(_pollfds[i].fd); // gelen veri işle
            }
        }
    }
}
