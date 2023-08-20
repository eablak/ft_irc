#include "../includes/Topic.hpp"
 Topic::Topic()
 {}
Topic::~Topic()
 {}
void Topic::execute(Server &server, Client &client)
{
    (void)server;
    (void)client;
    std::vector<std::string> params = client.getParams();
    if (params.size() != 1 && params.size() != 2)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("TOPIC")));
        return;
    }
    if(client.isInChannel(params[0]))
    {
        Channel &ch = server.getChannel(params[0]);
        if(params.size() == 1)
        {
            if(ch.getTopic() != "")
                Numeric::printNumeric(client, server, RPL_TOPIC(params[0], ch.getTopic()));
            else
                Numeric::printNumeric(client, server, RPL_NOTOPIC(params[0]));
        }
        else if (params.size() == 2)
        {   
            if(ch.getTopic() != "")
                ch.setTopic(params[1]);
            else
                ch.setTopic(params[1]);
            Numeric::printNumeric(client, server, RPL_TOPIC(params[0], params[1]));   
        }
    }
    else
        Numeric::printNumeric(client, server, ERR_NOTONCHANNEL(params[0]));
}
