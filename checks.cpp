#include "includes/checks.hpp"
using namespace std;


void checks::check_args(int ac){
    if (ac != 3){
        cerr << "Usage <port> <password>" << endl;
        exit(1);
    }
}