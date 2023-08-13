#include "includes/Numeric.hpp"
#include "includes/Server.hpp"

void Numeric::createNumeric(std::string errNo, std::string _define){
    numericMap[errNo] = _define;
}

void Numeric::printNumeric(std::string errNo, Client &client, Server &server){
    server.messageToClient(client.getClientFd(),numericMap[errNo]);
}

void Numeric::handleNumeric(std::string errNo,std::string _define, Client &client, Server &server){
    createNumeric(errNo,_define);
    printNumeric(errNo,client,server);
}