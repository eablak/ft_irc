#ifndef JOIN_HPP
#define JOIN_HPP
#include "Utils.hpp"
#include "ICommand.hpp"
class Join : public ICommand
{
private:
    bool checkChannelName(std::string &channelName);
    void sendSuccessNumerics(Server &server, Client *client,Channel &channel);
    void handleMultipleChannels(Server &server, Client *client, std::vector<std::string> &params);
	void handleWithParams(Server &server, Client *client,std::vector<std::string>&params);
public:
    Join();
    ~Join();
    void execute(Server &server, Client *client);
};
#endif
