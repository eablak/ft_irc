#include "../includes/Part.hpp"
#include <ctime>

Part::Part()
{
}

Part::~Part()
{
}

void Part::handleWithParams(Server &server, Client *client, std::vector<std::string> &params)
{
    if (params.size() != 2)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS(std::string("PART")));
        return;
    }
    if (!client->isInChannel(params[0]))
    {
        Numeric::printNumeric(client, server, ERR_NOTONCHANNEL(params[0]));
        return;
    }
    try
    {
        Channel &ch = server.getChannel(params[0]);
        ch.sendMessageToChannel(server,client, "PART " + ch.getName() + " :" + params[1]);

        std::cout << "ayrilan  client: " << client->getNickname() << std::endl;
        std::vector<Client *>::iterator it;
        int is_op = 0;
        int any_other_op = 0;
        for(it = ch.getOperators().begin(); it != ch.getOperators().end(); it++){
            if ((*it)->getNickname() == client->getNickname()){
                std::cout << "ayrilan kisi op" << std::endl;
                is_op = 1;
            }
            if ((*it)->getNickname() != client->getNickname()){
                std::cout << "baska op var " << (*it)->getNickname() << std::endl;
                any_other_op = 1;
            }
        }
        Client *new_op;
        if (is_op == 1 && any_other_op == 0 && ch.getClients().size() != 1){
            std::cout << "ayrilan kisi op ve baska op yok" << std::endl;
            std::srand(std::time(0));
            size_t rand_op = std::rand() % ch.getClients().size() + 1;
            for(size_t i = 0; i < ch.getClients().size(); i++){
                if (i == rand_op){
                    new_op = ch.getClients()[i];
                    std::cout << " new_op " << ch.getClients()[i]->getNickname() << std::endl;
                }
            }
            ch.addOperator(new_op);
        }


        ch.removeClient(client);
        client->removeChannel(ch);
    }
    catch (Server::ChannelNotFoundException &e)
    {
        Numeric::printNumeric(client, server, ERR_NOSUCHCHANNEL(params[0]));
    }
}
void Part::handleMultipleChannels(Server &server, Client *client, std::vector<std::string> &params)
{
    std::vector<std::string> channels = Utils::split(params[0], ',');
    std::vector<std::string>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        std::vector<std::string> newParams;
        newParams.push_back(*it);
        handleWithParams(server, client, newParams);
    }
}

void Part::execute(Server &server, Client *client)
{
    std::vector<std::string> params = Utils::concatParams(client->getParams());
    if (params.size() == 0)
    {
        Numeric::printNumeric(client, server, ERR_NEEDMOREPARAMS("PART"));
        return;
    }
    if (params[0].find(',') != std::string::npos)
    {
        handleMultipleChannels(server, client, params);
        return;
    }
    handleWithParams(server, client, params);
}