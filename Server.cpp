#include "includes/Server.hpp"

Server::Server(std::string _port, std::string _password)
{
	this->port = std::atoi(_port.c_str());
	this->password = _password;
	setHostname();
	createSocket();
}

void Server::createSocket()
{
	int opt;
	struct sockaddr_in serverAddr;

	opt = 1;
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error::error_func("Error socket failed");
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		error::error_func("Error setsocket options");
	if (fcntl(socketfd, F_SETFL, O_NONBLOCK) < 0)
		error::error_func("Error while setting socket flag options");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(this->port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	if (::bind(socketfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		error::error_func("Error binding socket.");
	if (listen(socketfd, 100) < 0)
		error::error_func("Error listen socket");
	std::cout << "Server listening " << port << " port.." << std::endl;
}

void Server::messageToClient(Client *sender, Client *target, std::string msg)
{
	std::string bf = sender->getPrefix() + " " + msg + "\r\n";
	if (send(target->getClientFd(), bf.c_str(), bf.size(), 0) < 0)
		error::error_func("Send Error");
	std::cout << "Message to client: " << bf << std::endl;
}

void Server::clientAccept()
{
	struct sockaddr_in client_addr;
	socklen_t len;
	int client_fd;
	pollfd poll_client;

	len = sizeof(client_addr);
	client_fd = accept(socketfd, (sockaddr *)&client_addr, &len);
	if (client_fd == -1)
		error::error_func("Error accept");
	else
	{
		poll_client.fd = client_fd;
		poll_client.events = POLLIN;
		poll_client.revents = 0;
		Client *client = new Client(client_fd, this->hostname);
		_pollfds.push_back(poll_client);
		_clients.push_back(client);
		std::cout << "fd " << client_fd << " client succesfully connected\n";
		messageToClient(client, client, "Welcome to IRC. Please Enter Password");
	}
}

Client *Server::getClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getClientFd() == fd)
			return (_clients[i]);
	}
	std::cout << "getClient Error" << std::endl;
	exit(1);
}

std::string Server::readMessage(int fd)
{
	int bytesRead;

	std::vector<char> buffer(5000);
	bytesRead = recv(fd, buffer.data(), buffer.size(), 0);
	if (bytesRead == -1)
	{
		std::cerr << "Receive error." << std::endl;
		exit(1);
	}
	if (bytesRead == 0)
		throw ClientDisconnectedException();
	buffer[bytesRead] = '\0';

	return std::string(buffer.data());
}

void Server::removeClient(Client *client)
{
	std::cout << "Client " << client->getNickname() << " disconnected" << std::endl;
	close(client->getClientFd());
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == client->getClientFd())
		{
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].isClientInChannel(client))
			_channels[i].removeClient(client);
	}
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getClientFd() == client->getClientFd())
		{
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

void Server::clientEvent(int fd)
{
	Client *client = getClient(fd);
	std::string msg;
	try
	{
		client->setMessage(client->getMessage() + readMessage(fd));
	}
	catch (ClientDisconnectedException &e)
	{
		removeClient(client);
	}
	if (client->getMessage().find("\n") == std::string::npos)
		return;
	msg = client->getMessage();
	std::cout << "Message from client: " << msg << std::endl;
	HandleMessage _handlemsg(client);
	while (msg.size() > 0)
	{
		if (!_handlemsg.handleMsg(*this, client, msg))
			return;
		ICommand *command = _handlemsg.getCommand(client->getCommand());
		if (command == NULL)
		{
			if (_handlemsg.checkAuthCommand(*this, client) == 1)
			{
				_handlemsg.removeExecutedPart(msg);
				continue;
			}
			Numeric::printNumeric(client, *this, ERR_UNKNOWNCOMMAND(client->getCommand()));
			_handlemsg.removeExecutedPart(msg);
			continue;
		}
		command->execute(*this, client);
		_handlemsg.removeExecutedPart(msg);
	}
	client->setMessage("");
}

void Server::serverInvoke()
{
	pollfd initalize = {socketfd, POLLIN, 0};
	_pollfds.push_back(initalize);
	while (1)
	{
		if (poll(&_pollfds[0], _pollfds.size(), 0) == -1)
			error::error_func("Error while polling");
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 0)
				continue;
			if (_pollfds[i].revents & POLLIN)
			{
				if (_pollfds[i].fd == socketfd)
					clientAccept();
				else
					clientEvent(_pollfds[i].fd);
			}
		}
	}
}

std::string Server::getPassword()
{
	return (password);
}

std::string Server::getHostname()
{
	return this->hostname;
}

void Server::setHostname()
{
	char hostname_c[1024];
	int rtn = gethostname(hostname_c, 1024);
	if (rtn == -1)
		error::error_func("Host Error\n");
	this->hostname = hostname_c;
}

std::vector<Client *> &Server::getClients()
{
	return (this->_clients);
}

std::vector<Channel> &Server::getChannels()
{
	return (this->_channels);
}

Channel &Server::getChannel(std::string &channelName)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == channelName)
			return (_channels[i]);
	}
	throw ChannelNotFoundException();
}
void Server::addChannel(std::string channelName, Client *client)
{
	_channels.push_back(Channel(channelName, client));
}
Client *Server::getClientByNickname(std::string nickname)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNickname() == nickname)
			return (_clients[i]);
	}
	throw ClientNotFoundException();
}

Server::~Server()
{
}

void Server::removeChannel(Channel &channel)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == channel.getName())
		{
			_channels.erase(_channels.begin() + i);
			break;
		}
	}
}