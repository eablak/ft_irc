#ifndef CHECKS_HPP
#define CHECKS_HPP
#include "librarys.hpp"

namespace checks{
    void check_args(int ac,char **av);
    int checkNick(std::string nick);
}

#endif