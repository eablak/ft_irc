#include "../includes/Server.hpp"


int main(int ac, char **av){
    checks::check_args(ac,av);
    Server server(av[1],av[2]);
    server.serverInvoke();
}