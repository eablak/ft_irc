#include "../includes/Pass.hpp"

PASS::PASS(){

}

PASS::~PASS(){}

void PASS::execute(Server &server, Client &client, std::list<std::pair<std::string, std::string> >client_message){
    (void) server;
    (void) client;
    std::cout << "size: " << client_message.size() << " "<< client_message.front().second << std::endl;

}