#include "includes/Server.hpp"


int main(int ac, char **av){
    checks::check_args(ac);
    Server server(av[1],av[2]);
}