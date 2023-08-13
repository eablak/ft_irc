#include "includes/Numeric.hpp"

void Numeric::createNumeric(std::string errNo, std::string _define){
    numericMap[errNo] = _define;
}

void Numeric::printNumeric(std::string errNo, Client &client){
    std::cout << numericMap[errNo] << std::endl;
    (void) client;
}

void Numeric::handleNumeric(std::string errNo,std::string _define, Client &client){
    createNumeric(errNo,_define);
    printNumeric(errNo,client);
}