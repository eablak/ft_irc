#include "includes/fileTransfer.hpp"
#include <fstream>

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}

void File::_sendFile(Server &server, Client *client){
    Client *sendClient = server.getClientWithNick(client->getParams()[4]);
    printf("Getting Picture Size\n");
    // make file stream
    FILE *picture;
    picture = fopen("server_image.png", "rb");

    // get size of file
    int size;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);

    //Send Picture Size
    printf("Sending Picture Size\n");
    char file_size[256];
    sprintf(file_size, "%d", size);
    std::cout << "Picture size:";
    std::cout << file_size << std::endl;
    send(sendClient->getClientFd(), file_size, sizeof(file_size), 0);

    // Send Picture as Byte Array(without need of a buffer as large as the image file)
    printf("Sending Picture as Byte Array\n");
    char send_buffer[BUFSIZ]; // no link between BUFSIZE and the file size
    std::cout << sizeof(send_buffer) << std::endl;
    printf("Send Start :\n");


    int counter = 0;
    while (!feof(picture))
    {
        int nb = fread(send_buffer, 1, sizeof(send_buffer), picture);
        send(sendClient->getClientFd(), send_buffer, nb, 0);
        std::cout << "Buffer Send ... " << std::endl;
        std::cout << "byte ";
        std::cout << nb << std::endl;
        counter += nb;
        // no need to bzero
    }
    std::cout << "Sent Size:";
    std::cout << counter << std::endl;
}

void File::_getFile(Server &server, Client *client){
    (void) server;

    // define buffer
    char *buff = new char[BUFSIZ];

    //Read Picture Size
    printf("Reading Picture Size\n");
    recv(client->getClientFd(), buff, BUFSIZ, 0);
    int file_size = atoi(buff);
    std::cout << "Picture size:";
    std::cout << file_size << std::endl;

    // create new file for recive image
    std::ofstream imageFile;
    imageFile.open("client_image.png", std::ios::binary);

    //Read Picture Byte Array and Copy in file
    printf("Reading Picture Byte Array\n");
    ssize_t len;
    int remain_data = file_size;
    int reciveddata = 0;
    printf("Recive Started:\n");
    while ((remain_data > 0) && ((len = recv(client->getClientFd(), buff, BUFSIZ, 0)) > 0))
    {
        imageFile.write(buff, len);
        reciveddata += len;
        remain_data -= len;
        int percent = (reciveddata*1.0 / file_size*1.0) * 100;
        std::cout << "Recive:";
        std::cout << percent;
        std::cout << "%" << std::endl;
    }

    // close stream
    imageFile.close();
    std::cout << "Recived Size:";
    std::cout << reciveddata << std::endl;
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