
#include "../includes/Mode.hpp"

Mode::Mode()
{
}

Mode::~Mode()
{
}
void Mode::checkMode(Server &server,Channel &channel,  Client *client,char mode)
{
	if(mode == TOPIC_LOCKED)
	{
		if(server.getChannel(client->getParams()[0]).isClientOperator(client))
			server.getChannel(client->getParams()[0]).addMode(TOPIC_LOCKED);
		else
			Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(client->getParams()[0]));
	}
	else if(mode == SECRET)
	{
		if(server.getChannel(client->getParams()[0]).isClientOperator(client))
			server.getChannel(client->getParams()[0]).addMode(SECRET);
		else
			Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(client->getParams()[0]));
	}
}
void Mode::handleClient(Server &server, Client *client, std::vector<std::string> &params)
{
	Client *clientToChange;
	try
	{
		clientToChange = server.getClientByNickname(params[0]);
	}
	catch (Server::ClientNotFoundException &e)
	{
		Numeric::printNumeric(client, server, ERR_NOSUCHNICK(params[0]));
		return;
	}
}
void Mode::handleChannel(Server &server, Client *client, std::vector<std::string> &params)
{
	try{
		Channel &channel = server.getChannel(params[0]);
		if(params.size() < 2)
		{
			Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(), channel.getMode()));
			return;
		}
		std::string modes = params[1].substr(1, params[1].size() - 1);
		for(size_t i = 0; i < modes.size(); i++)
		{
			if(modes[i] == '+')
				continue;
			else if(modes[i] == '-')
				continue;
			else
				checkMode(server,channel ,client ,modes[i]);
		}
	}
	catch(Server::ChannelNotFoundException &e)
	{
		Numeric::printNumeric(client, server, ERR_NOSUCHCHANNEL(params[0]));
		return;
	}
}
void Mode::execute(Server &server, Client *client)
{
	std::vector<std::string> params = client->getParams();
	if (params[0][0] == '#')
		handleChannel(server, client, params);

	else
		handleClient(server, client, params);
}
