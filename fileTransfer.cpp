#include "includes/fileTransfer.hpp"
#include <fstream>
#include <thread>
#define MAX_LINE 256

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

int _global = 0;

File::File(){
  
}

void _getFile(Server &server, Client *client){
    (void) server;
    (void) client;
}

void File::_sendFile(Server &server, Client *client) {
    (void) server;
    (void) client;
}



std::vector<int> clientSockets;

void handleClient(int clientSocket, int receiver) {

    // char buffer[1024];
    // int bytesRead;

    (void) clientSocket;
    FILE *in = fopen("background.jpeg","r");
    fseek(in, 0, SEEK_END); // Dosyanın sonuna git
    long file_size = ftell(in); // Dosyanın sonundan dosya boyutunu al
    fseek(in, 0, SEEK_SET); // Dosyanın başına geri dön

    // Buffer için bellek ayır
    unsigned char *buffer = (unsigned char *)malloc(file_size);
    if (buffer == NULL) {
        perror("Bellek ayirma hatasi");
        fclose(in);
        return ;
    }

    // Dosyayi buffer'a oku
    size_t bytes_read = fread(buffer, 1, file_size, in);
    if (bytes_read != (size_t)file_size) {
        perror("Dosya okuma hatasi");
        free(buffer);
        fclose(in);
        return ;
    }

    std::cout << "dosya bytee: " << bytes_read << std::endl;
    getchar();

    // while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // for (long unsigned int i = 0; i < clientSockets.size(); ++i) {
        //     if (clientSocket != clientSockets[i]) {
        //         send(clientSockets[i], buffer, bytes_read, 0);
        //     }
        // }
    // }

    fseek(in, 0, SEEK_SET); //!!!!!

    int okunan_total = 0;
    int send_total = 0;
    FILE *fp=fopen("recv.jpeg","w");
    // (void) fp;
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


void handle_error(int eno, char const *msg)
{
    if (eno == 0)
        std::cerr << msg << std::endl;
    else
        std::cerr << msg << ": " << strerror(eno) << std::endl;
    exit(errno);
}

void File::execute(Server &server, Client *client){
    std::cout << "!!   " << server.getHostname() << "   " << std::endl;
    if ((client->getParams().size() != 5) || (client->getParams()[2] != server.getHostname() || std::stoi(client->getParams()[3]) != server.getPort()))
    {
        server.messageToClient(client,client,"Missing parameter DCC");
        return;
    }


    int receiver = server.getClientByNickname(client->getParams()[4])->getClientFd();
    clientSockets.push_back(client->getClientFd());
    clientSockets.push_back(receiver);
    std::thread clientThread(handleClient, client->getClientFd(), receiver);
    clientThread.detach();
    return ;


    if (client->getParams()[0] == "SENDFILE")
        _sendFile(server, client);
    else if (client->getParams()[0] == "GETFILE")
        _getFile(server, client);
    else
        server.messageToClient(client,client,"Wrong command");
}