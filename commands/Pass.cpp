#include "../includes/Pass.hpp"

Pass::Pass(){

}

Pass::~Pass(){}

void Pass::execute(Server &server, Client &client){

    if(client.getAuthStatus() != NOTAUTHENTICATED){
        client.getNums().handleNumeric("462",ERR_ALREADYREGISTRED(),client,server);
        return;
    }
    if (client.getParams().empty()){
        client.getNums().handleNumeric("461",ERR_NEEDMOREPARAMS(client.getMap().front().second),client,server);
        return;
    }

    client.setParamsEnd();
    HandleMessage _handlmsg;
    // : doğru say size 1 se
    if (client.getParams()[0] == server.getPassword()){
        client.setAuthStatus(AUTHENTICATE);
        _handlmsg.removeParams(client);
        server.messageToClient(client.getClientFd(),"Password is correct!\n");
    }else {
        client.getNums().handleNumeric("464", ERR_PASSWDMISMATCH(),client,server);
        _handlmsg.removeParams(client);
    }
}
