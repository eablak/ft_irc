#include "../includes/User.hpp"

User::User(){

}

User::~User(){}

void User::execute(Server &server, Client &client){

    HandleMessage _handlemsg;

    std::vector<std::string> new_params;
    std::string all;
    for(size_t i = 0; i < client.getParams().size(); i++){
        if (client.getParams()[i][0] == ':'){
            while(i < client.getParams().size()){
                all += client.getParams()[i];
                if (i != client.getParams().size() - 1)
                    all += " ";
                i++;
            }
            new_params.push_back(all);
        }else{
            new_params.push_back(client.getParams()[i]);
        }
    }

    if (new_params.size() != 4 || (new_params[1] != "0" || new_params[2] != "*")){
        server.messageToClient(client.getClientFd(),"Error: Missing parameter\n");
        _handlemsg.removeParams(client);
        return ;
    }

    std::cout << "işle\n";

    _handlemsg.removeParams(client);
    new_params.clear();
}