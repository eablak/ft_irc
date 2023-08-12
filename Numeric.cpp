#include "includes/Numeric.hpp"

void numeric::createNumeric(std::string errorNo, std::string errorStr){
    
    numericList.push_back(std::make_pair(errorNo,errorStr));
}