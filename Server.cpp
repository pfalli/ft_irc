/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:34:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/19 16:23:35 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"
#include "Channel.hpp"
#include "NumericMessages.hpp"

const char* SeverExceptionSocket::what() const throw()
{
	return "Error: Socket.";
}

const char* SeverExceptionBind::what() const throw()
{
	return "Error: Bind.";
}

const char* SeverExceptionListen::what() const throw()
{
	return "Error: Listen.";
}

const char* FailedPollException::what() const throw()
{
	return "Error: Poll timeout or error.";
}

Server::Server(): \
	name("default"), password("123456\n"), port(8080)
{
	this->server_shutdown = false;
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		throw SeverExceptionSocket();
	}
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(port);
	this->serverAddress.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr);
	signal(SIGINT, signal_handler);
	global_server = this;
}

Server::Server(std::string _password, int _port, std::string _name): \
	name(_name), password(_password), port(_port)
{
	this->server_shutdown = false;
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		throw SeverExceptionSocket();
	}
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(port);
	this->serverAddress.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr); 	 // convert a number to an array of integers: 127.0.0.1

	signal(SIGINT, signal_handler);
	global_server = static_cast<Server *>(this);
	// std::cout << global_server << std::endl;
	signal(SIGKILL, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
}

Server::~Server()
{
}

Server::Server(const Server &src): name(src.getName()), \
	password(src.getPassword()), port(src.getPort())
{
	*this = src;
}

int							Server::getPort()const{return (port);}
std::string					Server::getName()const{return (name);}
std::string					Server::getPassword()const{return (password);}
std::vector<Client>			Server::getClients()const{return (clients);}
int							Server::getServSocket()const{return (serverSocket);}
std::vector<Channel>		Server::getChannels()const{return (channels);}
std::vector<Channel>& 		Server::getChannelsref() {return channels;}
bool						Server::getServerShutdown()const{return (server_shutdown);}
std::vector<struct pollfd>	Server::getPollFds()const{return (this->poll_fds);}

void						Server::setServerShutdown(){this->server_shutdown = true;}

Server					&Server::operator=(const Server &src)
{
	if (this == &src)
		return *this;
	if (this->channels.size() > 0)
		this->channels.erase(channels.begin(), channels.end());
	if (this->clients.size() > 0)
		this->clients.erase(clients.begin(), clients.end());
	std::vector<Client>::iterator _clients = src.getClients().begin();
	while (_clients != src.getClients().end())
	{
		this->clients.push_back(*_clients);
		_clients++;
	}
	std::vector<Channel>::iterator _channels = src.getChannels().begin();
	while (_channels != src.getChannels().end())
	{
		this->channels.push_back(*_channels);
		_channels++;
	}
	this->server_shutdown = src.getServerShutdown();
	return (*this);
}


bool					Server::existingUser(int clientSocket)
{
	std::vector<Client>::iterator it = this->clients.begin();

	while (it != clients.end())
	{
		if ((*it).getSocket() == clientSocket)
			return (true);
		it++;
	}
	return (false);
}

bool							Server::existingName(std::string name, int mode)
{
	if (!this->clients.size())
		return false;
	std::vector<Client>::iterator it = this->clients.begin();
	if (mode == USERNAME)
	{
		while (it != this->clients.end())
		{
			if (it->getUserName() == name)
				return true;
			it++;
		}
	}
	else if (mode == NICKNAME)
	{
		while (it != this->clients.end())
		{
			if (it->getNickName() == name)
				return true;
			it++;
		}
	}
	return false;
}

int	Server::NewClient(int new_socket)
{
	std::cout << "New client connected: " << new_socket << std::endl;

	Client _new(new_socket);
	// _new.setUserName(requestName(USERNAME, clientSocket));
	// _new.setNickName(requestName(NICKNAME, clientSocket));
	std::cout << "new_socket#0 : " << new_socket << std::endl;
	this->clients.push_back(_new);
	// Add new client to poll lists
	pollfd client_fd;
	client_fd.fd = new_socket;
	std::cout << "new_socket#1 : " << new_socket << std::endl;
	std::cout << "clients_socket#1 : " << _new.getSocket() << std::endl;
	client_fd.events = POLLIN;  // Monitor for incoming data
	poll_fds.push_back(client_fd);
	std::string str = WELCOME_MESSAGE;
	send(new_socket, str.c_str(), strlen(str.c_str()), 0);
	return (SUCCESS);
}

void		Server::deleteClient(std::vector<Client>::iterator client, std::vector<pollfd>::iterator poll)
{
	std::cout	<< "Client disconnected: "	<< (*client).getUserName() + "," <<
		" Socket: "	<< client->getSocket()				<< std::endl;
	close(client->getSocket());
	this->clients.erase(client);
	this->poll_fds.erase(poll);
	// if client inside one or multiple channels => delete
	deleteClientInsideChannels(*client);

}

void	Server::deleteClientInsideChannels(const Client &client) {
	std::vector<Channel>::iterator channelIt = channels.begin();
	for (; channelIt != channels.end(); ++channelIt) { // found channel
		std::vector<Client> &joinedClients = channelIt->getJoinedClients(); // take reference to joinedClients
		for (std::vector<Client>::iterator clientIt = joinedClients.begin(); clientIt != joinedClients.end(); ++clientIt) { // found client inside the channel
			if (clientIt->getSocket() == client.getSocket()) {
				joinedClients.erase(clientIt);
				// if client is OPERATOR, delete from operators container
				std::vector<Client>::iterator operatorIt = channelIt->getOperators().begin();
				while (operatorIt != channelIt->getOperators().end()) {
					if (operatorIt->getSocket() == client.getSocket()) {
						channelIt->getOperators().erase(operatorIt);
						break;
					}
					operatorIt++;
				}
				break; // break and check inside other channels
			}
		}
	}
}

int									Server::acceptClient()
{
	pollfd client;
	socklen_t addrlen = sizeof(client);

	int socket = accept(serverSocket, (struct sockaddr*)&client, &addrlen);
	if (socket < 0)
	{
		perror("Accept failed");
		return FAILURE ;
	}
	return socket;
}

void	Server::launch()
{
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw SeverExceptionBind();
	std::cout << "Password to join: " << this->password << std::endl;

	// Mark the socket for listenign in
	if (listen(serverSocket, SOMAXCONN) == -1)
		throw SeverExceptionListen();

	std::cout << "Server listening on port " << this->port << std::endl;

	//------------------------------------------------//
	//                Poll()                          //
	//------------------------------------------------//

	// to manage multiple clients
	pollfd server_poll_fd;
	server_poll_fd.fd = serverSocket;
	server_poll_fd.events = POLLIN;  // constant check for incoming connections
	this->poll_fds.push_back(server_poll_fd);

	while (!server_shutdown)
	{
		std::cout << "Server Shutdown: " << server_shutdown << std::endl;
		int activity = poll(&poll_fds[0], poll_fds.size(), -1);  // Wait indefinitely
		if (activity == FAILURE || activity == POLL_TIMEOUT)
		{
			if (errno == EINTR)
				break ;
			throw FailedPollException();
		}
		std::vector<pollfd>::iterator it = poll_fds.begin();
		for (size_t i = 0; i < poll_fds.size(); i++)
		{
			if (it->revents & POLLIN)
			{
				if (it->fd == serverSocket)
				{
					// Check for new client connections
					newConnection();
				}
				else
					existingConnection(it);
			}
			it++;
		}
		// getMessages();
	}
	if (server_shutdown)
		clean();
	close(serverSocket);
	return ;
}

void Server::newConnection()
{
	int new_socket = acceptClient();
	if (new_socket == FAILURE)
		return ;
	if (NewClient(new_socket) == FAILURE)
		return ;
}

void	Server::existingConnection(std::vector<pollfd>::iterator it)
{
	std::vector<Client>::iterator it_c = findObject(it->fd, this->clients);
	if (it_c == this->clients.end())
		return ;
	Client *client = &(*(findObject(it->fd, this->clients)));
	char buffer[BUFFER_SIZE];
	int bytes_read;
	memset(buffer, 0, sizeof(buffer));

	if (server_shutdown)
		return ;
	bytes_read = recv(client->getSocket(), buffer, BUFFER_SIZE, 0); // ***receiving message
	if (bytes_read <= FAILURE || bytes_read == 0)
	{
		deleteClient(findObject(it->fd, this->clients), it);
		return ;
	}
	else if (bytes_read > 0)
	{
		std::cout << client->getSocket() << std::endl;
		std::cout << buffer << std::endl;
		std::string str = buffer;
		memset(buffer, 0, BUFFER_SIZE);
		if (str == "end")
		{
			deleteClient(findObject(it->fd, this->clients), it);
			return ;
		}
		else
		{
			std::cout << "Received(existingConnection): " << str << std::endl;
			Command cmd;
			parseCommand(str, cmd);
			handleCommand(cmd, *client);
			std::cout << "-----------------------------------------------------" << std::endl;
		}
	}
}

void									Server::clean()
{
	std::vector<Client>::iterator client = this->clients.begin();
	while (client != this->clients.end())
	{
		std::cout << std::endl << client->getNickName() << " " << client->getSocket() << std::endl;
		send(client->getSocket(), "Server closed unexpectedly.\n", 29, 0);
		client++;
	}
	std::cout << std::endl;
}

template <typename T>
typename std::vector<T>::iterator		Server::findObject(int toFind, std::vector<T> &array)
{
	typename std::vector<T>::iterator it = array.begin();
	while (it != array.end())
	{
		if ((*it).getSocket() == toFind)
			break;
		it++;
	}
	return (it);
}

void					Server::_register(Client &client, const Command &cmd, int mode)
{
	if (cmd.parameter.empty())
	{
		const std::string str = ERR_NEEDMOREPARAMS(client.getUserName());
		send(client.getSocket(), str.c_str(), str.length(), 0);
		return ;
	}
	else if (!cmd.message.empty())
	{
		const std::string str = ERR_TOOMANYPARAMS(client.getUserName());
		send(client.getSocket(), str.c_str(), str.length(), 0);
		return ;
	}
	if (mode == PASSWORD)
	{
		if (this->password == cmd.parameter)
			client.setPW();
		else
			send(client.getSocket(), "Incorrect Password. Please try again.\n", 39, 0);
	}
	else if (mode == USERNAME)
	{
		if (!client.getPW())
		{
			send(client.getSocket(), "Please enter the password first.\n", 34, 0);
			return ;
		}
		if (!existingName(cmd.parameter, USERNAME))
		{
			client.setUserName(cmd.parameter);
			client.setUser();
		}
		else
		{
			send(client.getSocket(), "Username already taken. Please try again with a different user name.\n", 70, 0);
			return ;
		}
	}
	else if (mode == NICKNAME)
	{
		if (!client.getPW())
		{
			send(client.getSocket(), "Please enter the password first.\n", 34, 0);
			return ;
		}
		if (!existingName(cmd.parameter, NICKNAME))
		{
			client.setNickName(cmd.parameter);
			client.setNick();
		}
		else
		{
			send(client.getSocket(), "Nickname already taken. Please try again with a different nick name.\n", 70, 0);
			return ;
		}
	}
	if (client.getNick() == true && client.getPW() == true && client.getUser() == true)
	{
		client.setRegistered();
		send(client.getSocket(), "Registration complete!\n", 24, 0);
	}
}

void Server::parseCommand(const std::string &str, Command &cmd) {
	std::istringstream iss(str);
	iss >> cmd.command;	
	std::string word;
	cmd.hasMessage = 0;
	bool messageStarted = false;
	while (iss >> word) {
		if (word[0] == ':') {
			messageStarted = true;
			cmd.hasMessage = 1;
			cmd.message = word.substr(1);
			std::getline(iss, word);
			cmd.message += word;
			break;
		} else if (!messageStarted) {
			if (!cmd.parameter.empty()) {
				cmd.parameter += " ";
			}
			cmd.parameter += word;
		}
	}	
	std::cout << "\n-------------------\nParsed command: " << cmd.command << std::endl;
	std::cout << "Parameter: " << cmd.parameter << std::endl;
	std::cout << "Message: " << cmd.message << "\n-------------------\n" << std::endl;

	std::cout << "|" << cmd.message << "|" << std::endl;
}


void	Server::createChannel(Client &client, std::string name, int creatorFd)
{
	// // Check if a channel with the given name already exists
	// for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	// {
	// 	if (it->getName() == name)
	// 		return ;
	// }
	// // if does not exist, create a new one and add
	channels.push_back(Channel(client, name, creatorFd));
}

