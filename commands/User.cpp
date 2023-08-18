#include "../includes/User.hpp"

User::User(){

}

User::~User(){}

void User::execute(Server &server, Client &client){

    std::cout << "helloooo\n";
    
    (void) server;
    (void) client;
}