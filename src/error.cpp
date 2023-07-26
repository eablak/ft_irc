#include "../includes/error.hpp"

void error::error_func(string error_msg){
    { cerr << error_msg << endl; 
    exit(1); }
}