#include "includes/checks.hpp"
using namespace std;


void checks::check_args(int ac, char **av){
    if (ac != 3){
        cerr << "Usage <port> <password>" << endl;
        exit(1);
    }
    try{
        int _port = stoi(av[1]);    //-std=c++11 !!!
        if (_port < 1024 || _port > 65535){
            cerr << "wrong port range "<< endl;
            exit(1);
        }
    } catch(const exception &e){
        cerr << "wrong port" << endl;
    }
}