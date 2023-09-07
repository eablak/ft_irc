#include "includes/fileTransfer.hpp"
#include <fstream>

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}

void File::_sendFile(Server &server, Client *client){


    // std::ifstream _file("a.jpeg", std::ios::binary); 
    // std::cout << "here \n";
    // char byte;
    // while (_file.get(byte)) {

    //     std::cout << "Okunan Bayt: " << static_cast<int>(byte) << std::endl;
    // }


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

    // std::ifstream text_file("test.txt");
    // std::string text;
    // text_file.seekg(0, std::ios::end);
    // text.reserve(text_file.tellg());
    // text_file.seekg(0, std::ios::beg);
    // text.assign(
    //     std::istreambuf_iterator<char>(text_file),
    //     std::istreambuf_iterator<char>()
    // );
    // ssize_t bytes_sent = send(sendClient->getClientFd(), text.c_str(), text.size(), 0);
    // if (bytes_sent == -1) {
    //     perror("send");
    // } else {
    //     std::cout << bytes_sent << " bayt metin gönderildi." << std::endl;
    // }






    std::ifstream image_file(client->getParams()[1], std::ios::binary);
    if (!image_file) {
        std::cerr << "Dosya açılamadı!" << std::endl;
        return ;
    }

    image_file.seekg(0, std::ios::end);
    size_t image_size = image_file.tellg();
    image_file.seekg(0, std::ios::beg);

    char *buffer = new char[image_size]; 
    image_file.read(buffer, image_size); 

    ssize_t bytes_sent = send(sendClient->getClientFd(), buffer, image_size, 0);

    if (bytes_sent == -1) {
        perror("send");
    } else {
        std::cout << bytes_sent << " bayt resim gönderildi." << std::endl;
    }


    char buffer2[1024]; 
    while (1) {
        ssize_t bytes_received = recv(sendClient->getClientFd(), buffer2, sizeof(buffer2), 0);

        if (bytes_received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Bekleme durumunda olduğumuz için devam ediyoruz
                continue;
            } else {
                perror("recv");
                break; // Diğer hatalar için döngüyü kır
            }
        } else if (bytes_received == 0) {
            // Soket kapatıldı
            std::cout << "Soket kapatıldı." << std::endl;
            break;
        } else {
            std::cout << bytes_received << " bayt veri alındı." << std::endl;
            // Veriyi işleme devam edin
        }
    }


    // if (bytes_received == -1) {
    //     perror("recv");
    // } else {
    //     std::cout << bytes_received << " bayt veri alındı." << std::endl;

    //     std::ofstream received_file("alınan_dosya.jpg", std::ios::binary);
    //     received_file.write(buffer2, bytes_received);
    //     received_file.close();

    //     std::cout << "Alınan dosya kaydedildi." << std::endl;
    // }

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