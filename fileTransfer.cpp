#include "includes/fileTransfer.hpp"
#include <fstream>
#include <thread>
#include "includes/Utils.hpp"
#define MAX_LINE 256

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}


int File::permission(std::string sender, int receiver, Server &server, std::string file_name){

    Client *client = server.getClient(receiver);
    server.messageToClient(client,client,sender +" wants to send you " + file_name + " file!");
    char buff[1024];
    recv(receiver,buff,sizeof(buff),0);
    std::string msg = buff;
    size_t pos = msg.find("\r\n");
	if (pos == std::string::npos)
		pos = msg.find("\n");
	std::string executablePart = msg.substr(0, pos);
    std::cout << executablePart << std::endl;
	std::vector<std::string> params = Utils::split(executablePart, ' ');
    
    if (params[0] == "DCC" && params[1] == "GETFILE" && params[2] == file_name && params[3] == server.getHostname() && params[4] == std::to_string(server.getPort()) && params[5] == sender){
        return 1;
    }
    Client *_sender = server.getClientByNickname(sender);
    server.messageToClient(_sender,_sender,client->getNickname()+ " did not accept!\n");
    return 0;
}


std::vector<int> clientSockets;

void handleClient(int clientSocket, int receiver) {

    FILE *in = fopen("background.jpeg","r");
    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(file_size);
    if (buffer == NULL) {
        perror("Bellek ayirma hatasi");
        fclose(in);
        return ;
    }

    size_t bytes_read = fread(buffer, 1, file_size, in);
    if (bytes_read != (size_t)file_size) {
        perror("Dosya okuma hatasi");
        free(buffer);
        fclose(in);
        return ;
    }

    std::cout << "dosya bytee: " << bytes_read << std::endl;
   
    fseek(in, 0, SEEK_SET); //!!!!!

    int okunan_total = 0;
    int send_total = 0;
    FILE *fp=fopen("recv.jpeg","w");
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        std::cout << "bytes read " << bytes_read << std::endl;
        okunan_total += bytes_read;
            if (receiver != -1) {
                if (send(receiver, buffer, bytes_read, 0) == -1){
                    std::cout << "error \n";
                    break;
                }else{
                    std::cout << "send ile " <<  bytes_read << std::endl;
                    send_total += bytes_read;
                    fwrite(buffer,sizeof(buffer),1, fp);
                }
            }
    }
    if (okunan_total == send_total){
        std::cout << "ESIT GONDERILDI\n";
    }
    std::cout << "FINISH\n";

    // close(clientSocket);

	// for (auto it = clientSockets.begin(); it != clientSockets.end(); ++it) {
    //     if (*it == clientSocket) {
    //         clientSockets.erase(it);
    //         break;
    //     }
    // }


    	
}


void File::execute(Server &server, Client *client){

    std::cout << "!!   " << server.getHostname() << "   " << std::endl;

    if (client->getParams()[0] != "SENDFILE"){
        server.messageToClient(client,client,"You can only use SENDFILE command!\n");
        return ;
    }
    if ((client->getParams().size() != 5) || (client->getParams()[2] != server.getHostname() || std::stoi(client->getParams()[3]) != server.getPort()))
    {
        server.messageToClient(client,client,"Missing parameter DCC");
        return;
    }

    int receiver = server.getClientByNickname(client->getParams()[4])->getClientFd();
    if (receiver == client->getClientFd()){
        server.messageToClient(client,client, "You can not send file to yourself!\n");
        return ;
    }

    std::string file_name = client->getParams()[1];
    if (permission(client->getNickname(), receiver, server, file_name)){
        clientSockets.push_back(client->getClientFd());
        clientSockets.push_back(receiver);
        std::thread clientThread(handleClient, client->getClientFd(), receiver);
        clientThread.detach();
        return ;
    }
}