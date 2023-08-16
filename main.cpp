#include "includes/libraries.hpp"
#include "includes/Server.hpp"
#include "includes/checks.hpp"

// client disconnect olup tekrar connect olursa eski bilgilerden devam ediyo düzenle!

int main(int ac, char **av){

    checks::check_args(ac,av);
    Server server(av[1],av[2]);
    server.serverInvoke();
}