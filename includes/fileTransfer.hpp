#ifndef FILETRANSFER
#define FILETRANSFER
#include "Server.hpp"

class File : public ICommand{

    private:
    std::string filename;

    public:
    File();
    void execute(Server &server, Client *client);
    int permission(std::string sender,int receiver, Server &server, std::string file_name);
};


void *handleClient(void* args);
#endif