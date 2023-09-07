#include "includes/fileTransfer.hpp"
#include <fstream>

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}

void File::_sendFile(Server &server, Client *client){


    std::ifstream _file("test.txt", std::ios::binary); 
    std::cout << "here \n";
    char byte;
    while (_file.get(byte)) {

        std::cout << "Okunan Bayt: " << static_cast<int>(byte) << std::endl;
    }


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
}

void File::_getFile(Server &server, Client *client){
    (void) client;
    (void) server;
    std::cout << "get file" << std::endl;
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