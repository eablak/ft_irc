#include "includes/fileTransfer.hpp"
#include <fstream>

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}

void File::_sendFile(Server &server, Client *client){

    std::cout << "SEND file" << std::endl;
    Client *sendClient = server.getClientWithNick(client->getParams()[4]);
    if (!sendClient){
        server.messageToClient(client, client, "Wrong SendClient nickname");
        return ;
    }

    std::fstream ifs(client->getParams()[1] , std::fstream::in);
	if (ifs.fail()){
		printf("999 :Invalid file path\n");
        return ;
    }

    std::cout << sendClient->getNickname() << " kişiye send" << std::endl;

    std::ifstream _file(client->getParams()[1], std::ios::binary);
    if (!_file) {
        std::cerr << "Dosya açılamadı!" << std::endl;
        return ;
    }

    _file.seekg(0, std::ios::end);
    size_t file_size = _file.tellg();
    _file.seekg(0, std::ios::beg);

    std::cout << "FILE SIZE : " << file_size << std::endl;

    char *buffer = new char[file_size]; 
    // _file.read(buffer, file_size); 

    ssize_t bytes_sent = send(sendClient->getClientFd(), buffer, file_size, 0);

    if (bytes_sent == -1) {
        perror("send");
    } else {
        std::cout << bytes_sent << " bayt resim gönderildi." << std::endl;
    }
  
}

void File::_getFile(Server &server, Client *client){

    Client *senderClient = server.getClientWithNick(client->getParams()[4]);
    if (!senderClient){
        server.messageToClient(client, client, "Wrong SenderClient nickname");
        return ;
    }
    std::cout << "GETFILE" << std::endl;
    char buffer2[1024]; 
    ssize_t bytes_received2 = 0;
    while (1) {
        ssize_t bytes_received = recv(senderClient->getClientFd(), buffer2, sizeof(buffer2), 0);
        bytes_received2 += bytes_received;
        if (bytes_received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            } else {
                perror("recv");
                break; 
            }
        } else if (bytes_received == 0) {
            std::cout << "Soket kapatıldı." << std::endl;
            break;
        } else {
            std::cout << bytes_received << " bayt veri alındı." << std::endl;
        }
    }
    std::cout << "EN SON :: " << bytes_received2 << std::endl;
}

void File::execute(Server &server, Client *client){
    std::cout << "!!   " << server.getHostname() << "   " << std::endl;
    if ((client->getParams().size() != 5) || (client->getParams()[2] != server.getHostname() || std::stoi(client->getParams()[3]) != server.getPort()))
    {
        server.messageToClient(client,client,"Missing parameter DCC");
        return;
    }
    if (client->getParams()[0] == "SENDFILE")
        _sendFile(server, client);
    else if (client->getParams()[0] == "GETFILE")
        _getFile(server, client);
    else
        server.messageToClient(client,client,"Wrong command");
}