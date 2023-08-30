
#include "../includes/Mode.hpp"

Mode::Mode()
{
}

Mode::~Mode()
{
}

void Mode::handleChannel(Server &server, Client *client, std::vector<std::string> &params)
{
	try
	{
		Channel &channel = server.getChannel(params[0]);
		if (params.size() < 2)
		{
			Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(),"+nt"));
			return;
		}
		if (channel.isClientOperator(client) == false)
		{
			Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(channel.getName()));
			return;
		}
	}
	catch (Server::ChannelNotFoundException &e)
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
}
