#include "includes/HandleMessage.hpp"
#include "includes/Utils.hpp"
#include "includes/Pass.hpp"
#include "includes/Nick.hpp"
#include "includes/User.hpp"
#include "includes/Join.hpp"
#include "includes/Topic.hpp"
#include "includes/Part.hpp"
#include "includes/Privmsg.hpp"
#include "includes/Quit.hpp"
#include "includes/Cap.hpp"
#include "includes/Who.hpp"
#include "includes/Kick.hpp"
#include "includes/Mode.hpp"
#include "includes/Notice.hpp"
#include "includes/Ping.hpp"
#include "includes/Names.hpp"

HandleMessage::HandleMessage(Client *client)
{
	if (client->getAuthStatus() == NOTAUTHENTICATED)
		processNotAuthenticated();
	else if (client->getAuthStatus() == AUTHENTICATE)
		processAuthenticate();
	else
		processRegistered();
}

std::map<std::string, ICommand *> HandleMessage::getCommandMap()
{
	return (_commandMap);
}

void HandleMessage::processNotAuthenticated()
{
	_commandMap.insert(std::make_pair("USER", new User()));
	_commandMap.insert(std::make_pair("NICK", new Nick()));
	_commandMap.insert(std::make_pair("CAP", new Cap()));
	_commandMap.insert(std::make_pair("PING", new Ping()));
	_commandMap.insert(std::make_pair("QUIT", new Quit()));

}

void HandleMessage::processAuthenticate()
{

	_commandMap.insert(std::make_pair("TOPIC", new Topic()));
	_commandMap.insert(std::make_pair("KICK", new Kick()));
	_commandMap.insert(std::make_pair("PASS", new Pass()));
	_commandMap.insert(std::make_pair("NICK", new Nick()));
	_commandMap.insert(std::make_pair("USER", new User()));
	_commandMap.insert(std::make_pair("JOIN", new Join()));
	_commandMap.insert(std::make_pair("PART", new Part()));
	_commandMap.insert(std::make_pair("PRIVMSG", new Privmsg()));
	_commandMap.insert(std::make_pair("QUIT", new Quit()));
	_commandMap.insert(std::make_pair("WHO", new Who()));
	_commandMap.insert(std::make_pair("MODE", new Mode()));
	_commandMap.insert(std::make_pair("NOTICE", new Notice()));
	_commandMap.insert(std::make_pair("PING", new Ping()));
	_commandMap.insert(std::make_pair("names", new Names()));
}

void HandleMessage::processRegistered()
{
	_commandMap.insert(std::make_pair("PASS", new Pass()));
	_commandMap.insert(std::make_pair("NICK", new Nick()));
	_commandMap.insert(std::make_pair("USER", new User()));
	_commandMap.insert(std::make_pair("CAP", new Cap()));
	_commandMap.insert(std::make_pair("PING", new Ping()));
	_commandMap.insert(std::make_pair("QUIT", new Quit()));

}

int HandleMessage::handleMsg(Server &server, Client *client, std::string msg)
{
	if (msg.size() > 512)
	{
		Numeric::printNumeric(client, server, ERR_INPUTTOOLONG());
		msg[511] = '\r';
		msg[512] = '\n';
		msg.clear();
		return (0);
	}

	if (msg == "")
		return 0;
	size_t pos = msg.find("\r\n");
	if (pos == std::string::npos)
		pos = msg.find("\n");
	std::string executablePart = msg.substr(0, pos);
	if (executablePart == "")
		return 0;
	std::vector<std::string> params = Utils::split(executablePart, ' ');
	client->setCommand(params[0]);
	params.erase(params.begin());
	client->setParams(params);
	return (1);
}

ICommand *HandleMessage::getCommand(std::string command)
{
	std::map<std::string, ICommand *>::iterator it;
	it = this->_commandMap.find(command);
	if (it == this->_commandMap.end())
		return NULL;
	return it->second;
}

int HandleMessage::checkAuthCommand(Server &server, Client *client)
{

	std::vector<std::string> _allCommands;

	_allCommands.push_back("PASS");
	_allCommands.push_back("NICK");
	_allCommands.push_back("USER");
	_allCommands.push_back("JOIN");
	_allCommands.push_back("PART");
	_allCommands.push_back("TOPIC");
	_allCommands.push_back("PRIVMSG");

	if (client->getAuthStatus() == NOTAUTHENTICATED)
	{
		for (size_t i = 0; i < _allCommands.size(); i++)
		{
			if (client->getCommand() == _allCommands[i])
			{
				server.messageToClient(client, client, "Error: You can only send PASS");
				_allCommands.clear();
				return 1;
			}
		}
	}
	else if (client->getAuthStatus() == AUTHENTICATE)
	{
		for (size_t i = 0; i < _allCommands.size(); i++)
		{
			if (!(client->getCommand() == "NICK" || client->getCommand() == "USER") && client->getCommand() == _allCommands[i])
			{
				server.messageToClient(client, client, "Error: You can only send NICK or USER");
				_allCommands.clear();
				return 1;
			}
		}
		if (client->getCommand() == "NICK" || client->getCommand() == "USER")
		{
			_allCommands.clear();
			return 1;
		}
	}
	_allCommands.clear();
	return 0;
}

void HandleMessage::removeExecutedPart(std::string &msg)
{
	int deleteCount = 2;
	size_t pos = msg.find("\r\n");
	if (pos == std::string::npos)
	{
		pos = msg.find("\n");
		deleteCount = 1;
	}
	if (pos != std::string::npos)
		msg.erase(0, pos + deleteCount);
}

HandleMessage::~HandleMessage()
{
	std::map<std::string, ICommand *>::iterator it;
	for (it = _commandMap.begin(); it != _commandMap.end(); it++)
	{
		delete it->second;
	}
	_commandMap.clear();
}
