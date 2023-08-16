#include "includes/HandleMessage.hpp"
#include "includes/Utils.hpp"

std::map<std::string, ICommand *> HandleMessage::getCommandMap(){
	return (_commandMap);
}

void HandleMessage::processNotAuthenticated(Server &server, Client &client)
{
    // if (!client.getMap().empty())	
	// {
	// 	if (client.getMap().front().first == "PASS")
	// 		_commandMap.insert(std::make_pair("PASS",new PASS()));
	// 	else
	// 		server.messageToClient(client.getClientFd(),"Error: you can only send PASS\n");
    // }
	(void) client;
	(void) server;
	_commandMap.insert(std::make_pair("PASS",new PASS()));
}

void HandleMessage::processAuthenticate(Server &server, Client &client)
{
	if (!client.getMap().empty())
	{
		if (client.getMap().front().first != "NICK"
			&& client.getMap().front().first != "USER")
		{
			server.messageToClient(client.getClientFd(),
					"Error: you can only send NICK or USER\n");
		}
		else
		{
			client.setMapSecondEnd();
			if (client.getMap().front().first == "NICK")
			{
				checks::checkNick(client.getMap().front().second);
				client.setNickname(client.getMap().front().second);
			}
			else
				client.setUsername(client.getMap().front().second);
		}
	}
	if (!(client.getNickname() == "" || client.getUsername() == ""))
	{
		client.setAuthStatus(REGISTERED);
		std::cout << "registered" << std::endl;
	}
}

void HandleMessage::processRegistered(Server &server, Client &client)
{
    (void) server;
	(void)client;
	return ;
}

void HandleMessage::clientMsgProcess(Server &server, Client &client){
    if (client.getAuthStatus() == NOTAUTHENTICATED)
    	processNotAuthenticated(server,client);
    else if (client.getAuthStatus() == AUTHENTICATE)
    	processAuthenticate(server,client);
    else
    	processRegistered(server,client);
}

int HandleMessage::handleMsg(Server &server, Client &client, std::string msg){
    if (msg.size() > 512)
	{
		std::cout << "512" << std::endl; //kendisi handleliyo
		client.getNums().handleNumeric("417", "ERR_INPUTTOOLONG", client, server);
		msg[511] = '\r';
		msg[512] = '\n';
	}

	if (msg == "")
		return 0;

	size_t findPos = msg.find(' ');
	std::string first;
	std::string second;

    if (findPos == std::string::npos){
		client.setCommand(msg);
		return 1;
	}

    first = msg.substr(0, findPos);
	second = msg.substr(findPos + 1);
	client.setCommand(first);
	size_t firstNonSpace = second.find_first_not_of(" ");

    if (firstNonSpace != std::string::npos)
        second.erase(0, firstNonSpace);
    else
		second.clear();

	if(second[0] == ':')
		client.getParams().push_back(second);
	else
		client.setParams(Utils::split(second,' '));

    return (1);
}

ICommand *HandleMessage::getCommand(std::string command){
	std::map<std::string, ICommand *>::iterator it;
	it = this->_commandMap.find(command);
	//nulsa kontrol
	return it->second;
}