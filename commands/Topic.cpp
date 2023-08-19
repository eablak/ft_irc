#include "../includes/Topic.hpp"
 Topic::Topic()
 {}
Topic::~Topic()
 {}
void Topic::execute(Server &server, Client &client)
{
    (void)server;
    (void)client;
    std::cout << "here\n"
              << std::endl;
}
