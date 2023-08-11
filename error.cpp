#include "includes/error.hpp"

void error::error_func(std::string error_msg){
    std::cerr << error_msg << std::endl; 
    exit(1); 
}