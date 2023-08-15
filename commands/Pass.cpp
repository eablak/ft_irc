#include "../includes/Pass.hpp"

PASS::PASS(){

}

PASS::~PASS(){}

void PASS::execute(Server &server, Client &client){

    client.setMapSecondEnd();
    std::cout << client.getMap().front().second << std::endl;
    if (client.getMap().front().second == server.getPassword())
    {
        std::cout << "password başarılı" << std::endl; // çıktıyı değiştir
        client.setAuthStatus(AUTHENTICATE);
        return ;
    }
    else
    {
        client.getNums().handleNumeric("464", ERR_PASSWDMISMATCH(),
                client, server);
    }

}