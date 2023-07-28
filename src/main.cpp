#include "../includes/Server.hpp"

/*

    listen'dan sonra accept -> client gelirse POLLIN
    password check
    fd'sini fd container'inde topla
    fd'sine göre de kendisini oluştur (nick,username)
    oluşturduğun bu client'i client container'inde topla
    (birden fazla client ve channel var yani container'larda tut)
    client oluştu ama bunun authenticate'i var onu check'le

    hatalar için NUMERIC!!

    gelen clientfd'leri için revents durumlarına göre işlem yap
        yani POLLIN gelen fd'yi işle -> gelen komuta göre çalıştır
*/

int main(int ac, char **av){
    checks::check_args(ac,av);
    Server server(av[1],av[2]);
    server.serverInvoke();
}