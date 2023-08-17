#include "../includes/Pass.hpp"

Pass::Pass(){

}

Pass::~Pass(){}

void Pass::execute(Server &server, Client &client){

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
}