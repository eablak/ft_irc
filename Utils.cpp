#include "includes/Utils.hpp"

std::vector<std::string> Utils::split(std::string &str, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        result.push_back(token);
    }
    return result;
}