#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include "librarys.hpp"

std::list<std::pair<std::string, std::string> > numericList;


#define RPL_WELCOME numeric::createNumeric("001", ":Welcome to the Internet Relay Network ! @")

namespace numeric{


    void createNumeric(std::string errorNo, std::string errorStr);

}

#endif