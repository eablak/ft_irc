#include "../includes/Nick.hpp"

Nick::Nick(){

}

Nick::~Nick(){}

void Nick::execute(Server &server, Client &client){

    (void) server;

    if (client.getParams().empty()){
        client.getNums().handleNumeric("461",ERR_NONICKNAMEGIVEN(),client,server);
        return;
    }else{ // kullanılabilir nickname mi - daha önce alınış mı

        //alınmış mı
        for(size_t i = 0; i < server.getClients().size(); i++){
            if (server.getClients()[i].getNickname() == client.getParams()[0]){
                client.getNums().handleNumeric("433",ERR_NICKNAMEINUSE(client.getParams()[0]),client,server);
                return ;
            }
        }


        if (client.getParams()[0][0] == ':' && client.getParams().size() == 1){
            server.messageToClient(client.getClientFd(), "Error: no leading colon (:)\n");
            return ;
        }
        
        client.setNickname(client.getParams()[0]);
        std::cout << client.getParams()[0] << " atandı" << std::endl;

    }

}