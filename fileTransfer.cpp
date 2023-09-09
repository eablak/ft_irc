#include "includes/fileTransfer.hpp"
#include <fstream>

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}

void File::_sendFile(Server &server, Client *client) {
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

    // Define buffer
    char *buff = new char[BUFSIZ];

    // Read Picture Size
    printf("Reading Picture Size\n");
    ssize_t bytes_received = recv(client->getClientFd(), buff, BUFSIZ, 0);
    if (bytes_received <= 0) {
        // Handle error or connection closure
        perror("recv");
        delete[] buff;
        return;
    }

    // Null-terminate the received data
    buff[bytes_received] = '\0';

    int file_size = atoi(buff);
    std::cout << "Picture size: " << file_size << std::endl;

    // Create a new file for receiving the image
    std::ofstream imageFile;
    imageFile.open("client_image.png", std::ios::binary);

    // Read Picture Byte Array and Copy to the file
    printf("Reading Picture Byte Array\n");
    int remain_data = file_size;
    int received_data = 0;
    printf("Receive Started:\n");
    
    while (remain_data > 0) {
        ssize_t len = recv(client->getClientFd(), buff, BUFSIZ, 0);
        if (len <= 0) {
            // Handle error or connection closure
            perror("recv");
            break;
        }
        imageFile.write(buff, len);
        received_data += len;
        remain_data -= len;
        int percent = (received_data * 1.0 / file_size * 1.0) * 100;
        std::cout << "Received: " << percent << "%" << std::endl;
    }

    // Close the file stream
    imageFile.close();
    std::cout << "Received Size: " << received_data << " bytes" << std::endl;

    // Clean up the buffer
    delete[] buff;
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