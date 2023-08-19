#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "./libraries.hpp"
#include "./Client.hpp"
class Client;
class Channel
{
private:
    std::string name;
    std::string topic;
    std::vector<Client> clients;
    std::vector<Client> operators;

public:
    Channel(std::string name, Client &client);
    ~Channel();
    std::string getName() const;
    std::string getTopic() const;
    std::vector<Client> &getClients();
    std::vector<Client> &getOperators();
    void addClient(Client &client);
    void removeClient(Client &client);
    bool isClientInChannel(Client &client);
    bool isClientOperator(Client &client);
};
#endif