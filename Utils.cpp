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

std::vector<std::string>Utils::concatParams(std::vector<std::string> &params)
{
    std::vector<std::string> new_params;
    std::string all;
    for (size_t i = 0; i < params.size(); i++)
    {
        if (params[i][0] == ':')
        {
            while (i < params.size())
            {
                all += params[i];
                if (i != params.size() - 1)
                    all += " ";
                i++;
            }
            new_params.push_back(all);
            break;
        }
        else
            new_params.push_back(params[i]);
    }
    return new_params;
}