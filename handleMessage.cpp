#include "includes/handleMessage.hpp"

std::map<std::string, ICommand *> handleMessage::getCommandMap(){
	return (_commandMap);
}

void handleMessage::processNotAuthenticated(Server &server, Client &client)
{
    if (!client.getMap().empty())
	{
		if (client.getMap().front().first == "PASS"){
			_commandMap.insert(std::make_pair("PASS",new PASS()));
        }
		else{
			server.messageToClient(client.getClientFd(),"Error: you can only send PASS\n");
		}
    }

	//PASS içinde
	// if (!client.getMap().empty())
	// {
	// 	if (client.getMap().front().first != "PASS")
	// 		server.messageToClient(client.getClientFd(),
	// 				"Error: you can only send PASS\n");
	// 	else
	// 	{
	// 		client.setMapSecondEnd();
	// 		if (client.getMap().front().second == server.getPassword())
	// 		{
	// 			std::cout << "password başarılı" << std::endl;
	// 			client.setAuthStatus(AUTHENTICATE);
	// 			return ;
	// 		}
	// 		else
	// 		{
	// 			client.getNums().handleNumeric("464", ERR_PASSWDMISMATCH(),
	// 					client, server);
	// 		}x
	// 	}
	// }
}

void handleMessage::processAuthenticate(Server &server, Client &client)
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

void handleMessage::processRegistered(Server &server, Client &client)
{
    (void) server;
	(void)client;
	return ;
}

void handleMessage::clientMsgProcess(Server &server, Client &client){
    if (client.getAuthStatus() == NOTAUTHENTICATED)
    	processNotAuthenticated(server,client);
    else if (client.getAuthStatus() == AUTHENTICATE)
    	processAuthenticate(server,client);
    else
    	processRegistered(server,client);
}

int handleMessage::handleMsg(Server &server, Client &client, std::string msg){
    if (msg.size() > 512)
	{
		std::cout << "512" << std::endl; //kendisi handleliyo
		client.getNums().handleNumeric("417", "ERR_INPUTTOOLONG", client, server);
		msg[511] = '\r';
		msg[512] = '\n';
	}

	size_t findPos = msg.find(' ');
	std::string first;
	std::string second;

    if (findPos == std::string::npos)
        return (0);

    first = msg.substr(0, findPos);
	second = msg.substr(findPos + 1);
    
    if (second[0] != ':'){
        size_t f_pos = second.find(' ');
        if (f_pos != std::string::npos){
            second = second.substr(0,f_pos);
        }
    }
    client.setClientMessage(first, second);
    return (1);
	// ilk gelen şey bir komut değilse onun hatasını ver
	// if (findPos == std::string::npos)
	// { // ve bir komutsa bu hata komut değilse başka hataya göre ayarla
	// 	if (msg == "NICK")
	// 	{
	// 		client.getNums().handleNumeric("431", ERR_NONICKNAMEGIVEN(), client, server);
	// 		return (0);
	// 	}
	// 	client.getNums().handleNumeric("461", ERR_NEEDMOREPARAMS(msg), client, server);
	// 	return (0);
	// }
	// first = msg.substr(0, findPos);
	// second = msg.substr(findPos + 1);
	// client.setClientMessage(first, second);
}