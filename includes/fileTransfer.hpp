#ifndef FILETRANSFER
#define FILETRANSFER
#include "Server.hpp"

class File : public ICommand{

    private:
    std::string filename;

    public:
    File();
    void execute(Server &server, Client *client);
    void _sendFile(Server &server, Client *client);
    // void _getFile(Server &server, Client *client);
};

#endif