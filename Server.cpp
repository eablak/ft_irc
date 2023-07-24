#include "includes/Server.hpp"

void Server::server_args(string port, string _password){
     try{
        int _port = stoi(port);
        if (_port < 1024 || _port > 65535){
            cerr << "wrong port range "<< endl;
            exit(1);
        }
        this->port = _port;
        this->password = _password;
    } catch(const exception &e){
        cerr << "wrong port" << endl;
    }
}

Server::Server(string port, string _password){
    server_args(port,_password);
      int serverSocket, newSocket, n;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen;
    char buffer[1024];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
       //exit func
    }

    // Prepare the sockaddr_in structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
       //exit func
    }

    // Listen
    listen(serverSocket, 5);

    std::cout << "Server listening on port 8080..." << std::endl;

    // Accept connection
    clientAddrLen = sizeof(clientAddr);
    newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (newSocket == -1) {
        std::cerr << "Error accepting connection." << std::endl;
       //exit func
    }

    // Read message from client
    n = read(newSocket, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        std::cerr << "Error reading from socket." << std::endl;
       //exit func
    }

    buffer[n] = '\0';
    std::cout << "Message from client: " << buffer << std::endl;

    // Respond to client
    const char* response = "Hello, client! I got your message.";
    n = write(newSocket, response, strlen(response));
    if (n == -1) {
        std::cerr << "Error writing to socket." << std::endl;
       //exit func
    }

    // Close sockets
    close(newSocket);
    close(serverSocket);

}