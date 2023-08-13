#include "includes/Numeric.hpp"

void Numeric::createNumeric(std::string errNo, std::string _define){
    printf("%p\n",&numericMap);
    std::cout << errNo << " " << _define << std::endl;
    numericMap[errNo] = _define;
    std::cout << "atama " << errNo << std::endl;
    
}

void Numeric::printNumeric(std::string errNo, Server &server){
    printf("%p\n",&numericMap);
    std::cout << numericMap.size() << std::endl;
    std::cout << "yazılacak: " << numericMap[errNo] << std::endl;
    (void) server;
}