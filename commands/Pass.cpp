#include "../includes/Pass.hpp"

PASS::PASS(){

}

PASS::~PASS(){}

void PASS::execute(Server &server, Client &client){

    if (client.getParams().empty()){
        client.getNums().handleNumeric("461",ERR_NEEDMOREPARAMS(client.getMap().front().second),client,server);
        return;
    }

    client.setParamsEnd();

    if (client.getParams()[client.getParams().size() - 1] == server.getPassword()){
        client.setAuthStatus(AUTHENTICATE);
        server.messageToClient(client.getClientFd(),"Password is correct!\n");
    }else {
        client.getNums().handleNumeric("464", ERR_PASSWDMISMATCH(),client,server);
    }

    if (!client.getParams().empty()) {
        while (client.getParams().size() > 1) {
            client.getParams().pop_back();
        }
    }

}