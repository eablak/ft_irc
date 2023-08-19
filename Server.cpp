#include "includes/Server.hpp"

Server::Server(std::string _port, std::string _password)
{
	this->port = std::stoi(_port);
	this->password = _password;
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

void Server::messageToClient(int fd, std::string msg)
{
	if (send(fd, msg.c_str(), msg.size(), 0) < 0)
		error::error_func("Send Error");
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
		_pollfds.push_back(poll_client);
		_clients.push_back(Client(client_fd));
		std::cout << "fd " << client_fd << " client succesfully connected\n";
		messageToClient(client_fd, "Welcome to IRC. Please Enter Password\r\n");
	}

	setHostname();
}

Client &Server::getClient(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getClientFd() == fd)
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
		throw std::out_of_range("t");
	}
	if (bytesRead == 0)
	{
		std::cout << "fd " << fd << " disconnect" << std::endl;
		Client &client = getClient(fd);
		std::vector<std::string> &params = client.getParams();
		params.clear();
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].getClientFd() == fd)
			{
				_clients.erase(_clients.begin() + i);
			}
		}
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].fd == fd)
				_pollfds.erase(_pollfds.begin() + i);
		}
		close(fd);
	}
	if (buffer.data()[bytesRead - 1] == 10 || buffer.data()[bytesRead - 1] == 13)
		buffer.data()[bytesRead - 1] = '\0';
	return (buffer.data());
}

void Server::clientEvent(int fd)
{
	Client &client = getClient(fd);
	std::string msg;
	try{
		msg = readMessage(client.getClientFd());
	}catch (std::exception &e){
		return;
	}
	std::cout << "Message: " << msg << std::endl;
	HandleMessage _handlemsg;
	if (!_handlemsg.handleMsg(*this, client, msg))
		return;
	_handlemsg.clientMsgProcess(*this, client);
	ICommand *command = _handlemsg.getCommand(client.getCommand());
	if (command == NULL){
		if (_handlemsg.checkAuthCommand(*this, client) == 1){
			_handlemsg.removeParams(client);
			return ;
		}
		Numeric::printNumeric(client, *this, ERR_UNKNOWNCOMMAND(client.getCommand()));
		_handlemsg.removeParams(client);
		return ;
	}
	client.setParamsEnd();
	command->execute(*this, client);
	_handlemsg.removeParams(client);
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
{ // !
	char hostname_c[1024];
	int rtn = gethostname(hostname_c, 1024);
	if (rtn == -1)
		error::error_func("Host Error\n");
	this->hostname = hostname_c;
}

std::vector<Client > &Server::getClients(){
	return (this->_clients);
}
