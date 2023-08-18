#include "includes/HandleMessage.hpp"
#include "includes/Utils.hpp"
#include "includes/Pass.hpp"
#include "includes/Nick.hpp"
#include "includes/User.hpp"

std::map<std::string, ICommand *> HandleMessage::getCommandMap(){
	return (_commandMap);
}

void HandleMessage::processNotAuthenticated(Server &server, Client &client)
{
	(void) server;
   	if (client.getCommand() != "PASS")
		return ;
	_commandMap.insert(std::make_pair("PASS",new Pass()));
}

void HandleMessage::processAuthenticate(Server &server, Client &client)
{

	if (client.getCommand() != "NICK" && client.getCommand() != "USER")
		return ;

	if (client.getCommand() == "NICK")
		_commandMap.insert(std::make_pair("NICK",new Nick()));
	if (client.getCommand() == "USER")
		_commandMap.insert(std::make_pair("USER",new User()));
	(void) server;
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
	 if (it == this->_commandMap.end()) {
        return NULL;
    }
	return it->second;
}

int HandleMessage::checkAuthCommand(Server &server, Client &client){

	std::vector<std::string> _allCommands;
	HandleMessage _handlemsg;

	_allCommands.push_back("PASS");
	_allCommands.push_back("NICK");
	_allCommands.push_back("USER");
	_allCommands.push_back("JOIN");
	_allCommands.push_back("PART");
	_allCommands.push_back("TOPIC");
	_allCommands.push_back("PRIVMSG");

	if (client.getAuthStatus() == NOTAUTHENTICATED){
		for(size_t i = 0; i < _allCommands.size(); i++){
			if (client.getCommand() == _allCommands[i]){
				server.messageToClient(client.getClientFd(),"Error: You can only send PASS\n");
				_allCommands.clear();
				return 1;
			}
		}
	}else if (client.getAuthStatus() == AUTHENTICATE){
		for(size_t i = 0; i < _allCommands.size(); i++){

			if (client.getCommand() == "PASS"){
				client.getNums().handleNumeric("462",ERR_ALREADYREGISTRED(),client,server);
				_handlemsg.removeParams(client);
				_allCommands.clear();
				return 1;
			}
			if (!(client.getCommand() == "NICK" || client.getCommand() == "USER") && client.getCommand() == _allCommands[i]){
				server.messageToClient(client.getClientFd(),"Error: You can only send NICK or USER\n");
				_allCommands.clear();
				return 1;
			}
		}
		if (client.getCommand() == "NICK" || client.getCommand() == "USER"){
			_allCommands.clear();
			return 1;
		}
	}else if (client.getAuthStatus() == REGISTERED){
		if (client.getCommand() == "USER"){
				client.getNums().handleNumeric("462",ERR_ALREADYREGISTRED(),client,server);
				_handlemsg.removeParams(client);
				_allCommands.clear();
				return 1;
			}
	}
	_allCommands.clear();
	return 0;
}

void HandleMessage::removeParams(Client &client){
	while (!client.getParams().empty()) {
		// std::cout << "Silinen değer " << client.getParams().back() << std::endl;
        client.getParams().pop_back();
    }
}