#include "checks.hpp"

void checks::check_args(int ac, char **av){
    if (ac != 3){
        std::cerr << "Usage <port> <password>" << std::endl;
        exit(1);
    }
    
}