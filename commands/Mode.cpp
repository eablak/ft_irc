
#include "../includes/Mode.hpp"

Mode::Mode()
{
}

Mode::~Mode()
{
}
void Mode::removeModes(Server &server, Channel &channel, Client *client, std::string &modes)
{
	for (size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i] == TOPIC_LOCKED)
		{
			if (channel.checkMode(TOPIC_LOCKED))
			{
				channel.removeMode(TOPIC_LOCKED);
				Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(), channel.getMode()));
			}
			else
			{
				Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(channel.getName()));
			}
		}
		else if (modes[i] == 's')
		{
			if (channel.checkMode(SECRET))
			{
				channel.removeMode(SECRET);
				Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(), channel.getMode()));
			}
			else
			{
				Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(channel.getName()));
			}
		}
	}
}
void Mode::addModes(Server &server, Channel &channel, Client *client, std::string &modes)
{
	for (size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i] == TOPIC_LOCKED)
		{
			if (!channel.checkMode(TOPIC_LOCKED))
			{
				channel.addMode(TOPIC_LOCKED);
				Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(), channel.getMode()));
			}
		}
		else if (modes[i] == SECRET)
		{
			if (!channel.checkMode(SECRET))
			{
				channel.addMode(SECRET);
				Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(), channel.getMode()));
			}
		}
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
	try
	{
		Channel &channel = server.getChannel(params[0]);
		if (params.size() < 2)
		{
			Numeric::printNumeric(client, server, RPL_CHANNELMODEIS(channel.getName(), channel.getMode()));
			return;
		}
		if (channel.isClientOperator(client) == false)
		{
			Numeric::printNumeric(client, server, ERR_CHANOPRIVSNEEDED(channel.getName()));
			return;
		}
		std::string modes = params[1].substr(1, params[1].size() - 1);
		if (modes.find('+') != std::string::npos)
			addModes(server, channel, client, modes);
		else if (modes.find('-') != std::string::npos)
			removeModes(server, channel, client, modes);
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

	else
		handleClient(server, client, params);
}
