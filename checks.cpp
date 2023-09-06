#include "includes/checks.hpp"

void checks::check_args(int ac, char **av){
    if (ac != 3){
        std::cerr << "Usage <port> <password>" << std::endl;
        exit(1);
    }
    try{
        int _port = std::atoi(av[1]);
        if (_port < 1024 || _port > 65535){
            std::cerr << "wrong port range "<< std::endl;
            exit(1);
        }
    } catch(const std::exception &e){
        std::cerr << "wrong port" << std::endl;
    }
}