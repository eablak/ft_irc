#include "../includes/Nick.hpp"

Nick::Nick(){

}

Nick::~Nick(){}

void Nick::execute(Server &server, Client &client){

    (void) server;
    (void) client;
    std::cout << "nick execut" << std::endl;
}