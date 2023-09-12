#include "includes/fileTransfer.hpp"
#include <fstream>
#define MAX_LINE 256

// DCC SEND <filename> <host> <port>
// DCC SENDFILE <filename> <host> <port> nickname/channel
// DCC GETFILE <filename> <host> <port> nickname/channel

File::File(){
  
}

void File::_sendFile(Server &server, Client *client) {


    int Socket = client->getClientFd();
    //hedef server
    struct sockaddr_in Server_Address = server.getServerAddr();
    connect ( Socket, (struct sockaddr *)&Server_Address, sizeof (Server_Address) );


    FILE *in = fopen("background.jpeg","r");
    char Buffer[2] = "";
    int len;
    int i = 0;
    while ((len = fread(Buffer,sizeof(Buffer),1, in)) > 0)
    {     
        std::cout << "i: " << i << std::endl;
        i++;
        send(Socket,Buffer,sizeof(Buffer),0);            
    }
    send(Socket,"Hi",sizeof(Buffer),0);

    char Buf[BUFSIZ];
    recv(Socket, Buf, BUFSIZ, 0);
    if ( strcmp (Buf,"ACK") == 0  )
    {
        printf("Recive ACK\n");
    }        
    // fclose(in);



    // -------------------------------------------------------------


    int Communication_Socket = server.getSocketFd();
    sockaddr_in Client_Address = client->getClinetAdrr();
    int Main_Socket = server.getSocketFd();

    socklen_t Lenght = sizeof (Client_Address);
    Communication_Socket = accept ( Main_Socket, (struct sockaddr*)&Client_Address, &Lenght );

    FILE *fp=fopen("recv.jpeg","w");
    while(1)
    {
        char Buffer[2]="";
        if (recv(Communication_Socket, Buffer, sizeof(Buffer), 0))
        {
            if ( strcmp (Buffer,"Hi") == 0  )
            {
                break;
            }
            else
            {
                fwrite(Buffer,sizeof(Buffer),1, fp);
            }
        }
    }
    fclose(fp);
    send(Communication_Socket, "ACK" ,3,0);
    printf("ACK Send");
    exit(0);

}

void handle_error(int eno, char const *msg)
{
    if (eno == 0)
        std::cerr << msg << std::endl;
    else
        std::cerr << msg << ": " << strerror(eno) << std::endl;
    exit(errno);
}

void File::_getFile(Server &server, Client *client){

    // int Communication_Socket = server.getSocketFd();
    // sockaddr_in Client_Address;
    // int Main_Socket;

    // socklen_t Lenght = sizeof (Client_Address);
    // Communication_Socket = accept ( Main_Socket, (struct sockaddr*)&Client_Address, &Lenght );



    // FILE *fp=fopen("recv.jpeg","w");
    // while(1)
    // {
    //     char Buffer[2]="";
    //     if (recv(Communication_Socket, Buffer, sizeof(Buffer), 0))
    //     {
    //         if ( strcmp (Buffer,"Hi") == 0  )
    //         {
    //             break;
    //         }
    //         else
    //         {
    //             fwrite(Buffer,sizeof(Buffer),1, fp);
    //         }
    //     }
    // }
    // fclose(fp);
    // send(Communication_Socket, "ACK" ,3,0);
    // printf("ACK Send");
    // exit(0);
    (void) server;
    (void) client;
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