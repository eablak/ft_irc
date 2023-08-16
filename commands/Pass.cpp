#include "../includes/Pass.hpp"

PASS::PASS(){

}

PASS::~PASS(){}

void PASS::execute(Server &server, Client &client){

    std::cout << "paSS" << std::endl;
    (void) server;
    (void) client;

    // client.setMapSecondEnd();
    // if (client.getMap().front().second == ""){
    //     client.getNums().handleNumeric("461",ERR_NEEDMOREPARAMS(client.getMap().front().second),client,server);
    //     return ;
    // }
    // if (client.getMap().front().second == server.getPassword())
    // {
    //     client.setAuthStatus(AUTHENTICATE);
    //     std::cout << "auth oldu" << std::endl;
    //     return ;
    // }
    // else
    // {
    //     client.getNums().handleNumeric("464", ERR_PASSWDMISMATCH(),
    //             client, server);
    //     return ;
    // }
}