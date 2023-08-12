#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "librarys.hpp"



#define ERR_NEEDMOREPARAMS numeric::makeNumeric("461", " :Not enough parameters")

class Numeric{
    private:
    std::map<std::string, std::string> numericMap;

    public:
    void createNumeric(std::string errorNo, std::string errorStr);
    void printNumeric(std::string errno);
};

#endif



//class tut