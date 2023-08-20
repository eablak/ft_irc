#include "./libraries.hpp"
#ifndef UTILS_HPP
#define UTILS_HPP
class Utils
{
public:
    static std::vector<std::string> split(std::string &str, char delim);
    static std::vector<std::string> concatParams(std::vector<std::string> &params);
};
#endif