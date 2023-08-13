#include "includes/Numeric.hpp"

void Numeric::createNumeric(std::string errNo, std::string _define){
    numericMap[errNo] = _define;
}

void Numeric::printNumeric(std::string errNo, Server &server){
    std::cout << numericMap[errNo] << std::endl;
    (void) server;
}

void Numeric::handleNumeric(std::string errNo,std::string _define, Server &server){
    createNumeric(errNo,_define);
    printNumeric(errNo,server);
}