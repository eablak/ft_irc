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

        

        if (client.getParams()[0][0] == ':' && client.getParams().size() == 1){
            server.messageToClient(client.getClientFd(), "Error: no leading colon (:)\n");
            return ;
        }
        
        client.setNickname(client.getParams()[0]);
        std::cout << client.getParams()[0] << " atandı" << std::endl;

    }

}