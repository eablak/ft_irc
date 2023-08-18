#include "../includes/Nick.hpp"

Nick::Nick(){

}

Nick::~Nick(){}

void Nick::execute(Server &server, Client &client){

    if (client.getParams().empty()){
        client.getNums().handleNumeric("461",ERR_NONICKNAMEGIVEN(),client,server);
        return;
    }else{
        HandleMessage _handlemsg;

        for(size_t i = 0; i < server.getClients().size(); i++){
            if (server.getClients()[i].getNickname() == client.getParams()[0]){
                client.getNums().handleNumeric("433",ERR_NICKNAMEINUSE(client.getParams()[0]),client,server);
                _handlemsg.removeParams(client);
                return ;
            }
        }

        if ((client.getParams()[0][0] == ':' && client.getParams().size() == 1) || client.getParams().size() != 1){
            server.messageToClient(client.getClientFd(), "Error: no leading colon (:)\n");
            _handlemsg.removeParams(client);
            return ;
        }

        try{
            std::stoi(std::string(1,client.getParams()[0][0]));
            client.getNums().handleNumeric("433",ERR_ERRONEUSNICKNAME(client.getParams()[0]),client,server);
            _handlemsg.removeParams(client);
            return ;
        }catch(std::exception &e){}

        for(unsigned long i = 0; i < client.getParams()[0].size(); i++){
            if (!std::isprint(client.getParams()[0][i])){
                client.getNums().handleNumeric("433",ERR_ERRONEUSNICKNAME(client.getParams()[0]),client,server);
                _handlemsg.removeParams(client);
                return ;
            }
        }

        if (client.getNickname().size() != 0){
            std::string msg = client.getNickname() + " changed his nickname to " + client.getParams()[0] + "\n";
            server.messageToClient(client.getClientFd(),msg);
            client.setNickname(client.getParams()[0]);
            _handlemsg.removeParams(client);
            return ;
        }
        
        client.setNickname(client.getParams()[0]);
        server.messageToClient(client.getClientFd(),"Requesting the new nick \"" + client.getNickname() + "\".\n");
        _handlemsg.removeParams(client);
    }
}