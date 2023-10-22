#include "includes/fileTransfer.hpp"
#include <fstream>
#include <thread>
#include "includes/Utils.hpp"
#include <errno.h>
#include <pthread.h> 
#define MAX_LINE 256

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname
// DCC GETFILE <filename> <host> <port> nickname

//std::c++11

struct ThreadArgs {
    int clientSocket;
    int receiver;
    std::string file_name;
};

File::File(){
  
}


int File::permission(std::string sender, int receiver, Server &server, std::string file_name){

    Client *client = server.getClient(receiver);
    server.messageToClient(client,client,sender +" wants to send you " + file_name + " file!");
    char buff[1024] = "";
    while (true) {
    int ret_recv = recv(receiver, buff, sizeof(buff), 0);
    if (ret_recv == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Veri mevcut değil, bir süre bekle ve tekrar dene.
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Örnek olarak 100 milisaniye bekleyin.
            continue;
        } else {
            // Diğer hata durumları için işlem yap.
            perror("recv");
            break;
        }
    } else {
        // Veri başarıyla alındı.
        break;
    }
}

    std::string msg = buff;
    size_t pos = msg.find("\r\n");
	if (pos == std::string::npos)
		pos = msg.find("\n");
	std::string executablePart = msg.substr(0, pos);
    // std::cout << "EXECUTABLE PART :" << executablePart << std::endl;
	std::vector<std::string> params = Utils::split(executablePart, ' ');
    std::cout << "!'!'!'! " << params[0] << params[1] << params[2] << params[3] << params[4] << params[5]<< std::endl;

    if (params[0] == "DCC" && params[1] == "GETFILE" && params[2] == file_name 
        && params[3] == server.getHostname() && params[4] == std::to_string(server.getPort()) 
            && params[5] == sender){
        return 1;
    }
    Client *_sender = server.getClientByNickname(sender);
    server.messageToClient(_sender,_sender,client->getNickname()+ " did not accept!\n");
    return 0;
}


void *handleClient(void* args) {

    ThreadArgs* clientArgs = static_cast<ThreadArgs*>(args);
    int clientSocket = clientArgs->clientSocket;
    int receiver = clientArgs->receiver;
    std::string file_name = clientArgs->file_name;
    (void) clientSocket;
    FILE *in = fopen(&file_name[0],"r");

    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    fseek(in, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(file_size);
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(in);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, in);
    if (bytes_read != (size_t)file_size) {
        perror("File reading error");
        free(buffer);
        fclose(in);
        return NULL;
    }

    std::cout << "dosya bytee: " << bytes_read << std::endl;
   
    fseek(in, 0, SEEK_SET); //!!!!!


    size_t pos = file_name.find(".");
    int len = file_name.length();
    std::string _part = file_name.substr(pos,len);
    std::string new_file = "recv" + _part;

    int okunan_total = 0;
    int send_total = 0;
    FILE *fp=fopen(&new_file[0],"w");
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
    return NULL;
}


void File::execute(Server &server, Client *client){

    // std::cout << "!! HOSTNAME  " << server.getHostname() << "   " << std::endl;

    std::string _file = client->getParams()[1];
    FILE *_in = fopen(&_file[0],"r");
    if (_in == NULL){
        server.messageToClient(client,client,"There is no " + _file + " file!\n");
        return;
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

        ThreadArgs *clientArgs = new ThreadArgs;
        clientArgs->clientSocket = client->getClientFd();
        clientArgs->file_name = client->getParams()[1];
        clientArgs->receiver = receiver;

        pthread_t ptid; 
        pthread_create(&ptid, NULL, &handleClient, clientArgs); 
        pthread_join(ptid,NULL);
        // std::thread clientThread(handleClient, client->getClientFd(), receiver, client->getParams()[1]);
        // clientThread.detach();
        return ;
    }
}