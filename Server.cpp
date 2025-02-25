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
std::vector<Channel>		Server::getChannels()const{return (channels);}
std::vector<Channel>& 		Server::getChannelsref() {return channels;}
bool						Server::getServerShutdown()const{return (server_shutdown);}

Server					&Server::operator=(const Server &src)
{
	if (this == &src)
		return *this;
	this->channels.erase(channels.begin(), channels.end());
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

int	Server::NewClient(int new_socket)
{
	std::cout << "New client connected: " << new_socket << std::endl;
	
	Client _new(new_socket);
	// _new.setUserName(requestName(USERNAME, clientSocket));
	// _new.setNickName(requestName(NICKNAME, clientSocket));
	this->clients.push_back(_new);
	// Add new client to poll lists
	pollfd client_fd;
	client_fd.fd = new_socket;
	client_fd.events = POLLIN;  // Monitor for incoming data
	poll_fds.push_back(client_fd);
	return (SUCCESS);
}

void		Server::deleteClient(std::vector<Client>::iterator client, std::vector<pollfd>::iterator poll)
{
	std::cout	<< "Client disconnected: "	<< (*client).getUserName() + "," <<
		" Socket: "	<< client->getSocket()				<< std::endl;
	close(client->getSocket());
	this->clients.erase(client);
	this->poll_fds.erase(poll);
}

bool									Server::sendToNext(std::string buff, int client_fd)
{
	if (this->clients.size() < 2){
		std::cout << "***only one client***\n"; 
		return false;
	}
	std::vector<Client>::iterator it = this->clients.begin();
	std::vector<Client>::iterator send_to;
	while (it != this->clients.end())
	{
		if ((*it).getSocket() == client_fd)
			break ;
		it++;
	}
	if (it + 1 == this->clients.end())
		send_to = (it - 1);
	else
		send_to = (it + 1);
	//complicated i know, couldnt think of anything else
	std::string temp = (*it).getNickName() + ": " + buff + "\n"; 
	const char *message = temp.c_str();
	send((*send_to).getSocket(), message, strlen(message), 0);
	return true;
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

bool Server::registration(std::vector<Client>::iterator it)
{

	char buffer[BUFFER_SIZE];
	int	bytes_read = recv(it->getSocket(), buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
		return false;
	buffer[bytes_read] = '\0';

	std::string input(removeNewline(buffer));
	if (input == password)
	{
		it->setPW();
		send(it->getSocket(), "Password correct. Please enter your NICK: \n", 45, 0);
	}
	else if (input != password)
	{
		send(it->getSocket(), "Incorrect password. Closing connection..\n", 42, 0);
		return false;
	}

	while (!it->getRegistered())
	{
		char buffer[BUFFER_SIZE];
		int	bytes_read = recv(it->getSocket(), buffer, BUFFER_SIZE, 0);
		if (bytes_read <= 0)
			return false;
		buffer[bytes_read] = '\0';
	
		std::string input(removeNewline(buffer));

		if (it->getNickName() == "default_nick")
		{
			if (input.empty())
				continue;
			else if (!validFormat(NICKNAME,input))
			{
				send(it->getSocket(), "Incorrect format. Please try again.\n", 37, 0);
				continue ;
			}
			else
			{
				it->setNickName(input);
				send(it->getSocket(), "Please enter USER:\n", 20, 0);
				continue ;
			}
		}
		else if (it->getUserName() == "default")
		{
			if (input.empty())
				continue;
			else if (!validFormat(USERNAME,input))
			{
				send(it->getSocket(), "Incorrect format. Please try again.\n", 37, 0);
				continue ;
			}
			else
			{
				it->setUserName(input);
				send(it->getSocket(), "Registration completed.\n", 24, 0);
				it->setRegistered();
				return true;
			}
		}
	}
	return (false);
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

	if (!it_c->getRegistered())
	{
		if (!registration(it_c))
		{
			std::cout << "Client disconnected: " << it_c->getNickName() << std::endl;
			deleteClient(it_c, it);
			return ;
		}
	}

	bytes_read = recv(client->getSocket(), buffer, BUFFER_SIZE, 0); // ***receiving message
	if (bytes_read <= FAILURE || bytes_read == 0)
	{
		deleteClient(findObject(it->fd, this->clients), it);
		return ;
	}
	else if (bytes_read > 0)
	{
		std::cout << buffer << std::endl;
		std::string str = buffer;
		memset(buffer, 0, BUFFER_SIZE);
		if (str == "end")
		{
			deleteClient(findObject(it->fd, this->clients), it);
			return ;
		}
		// else if(!sendToNext(str.c_str(), client->getSocket()))
		// 	std::cout << "Received(existingConnection): " << str << std::endl;
		else
		{
			std::cout << "Received(existingConnection): " << str << std::endl;
			Command cmd;
			parseCommand(str, cmd);
			handleCommand(cmd);
		}
	}
}


void Server::parseCommand(const std::string &str, Command &cmd) {
	std::istringstream iss(str);
	iss >> cmd.command;	
	std::string word;
	bool messageStarted = false;
	while (iss >> word) {
		if (word[0] == ':') {
			messageStarted = true;
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
}



void Server::handleCommand(const Command &cmd) {
	if (cmd.command == "JOIN") {
		std::cout << "Handling JOIN: " << cmd.parameter + cmd.message<< std::endl;
	} else if (cmd.command == "MODE") {
		std::cout << "Handling MODE: "<< cmd.parameter + cmd.message<< std::endl;
	} else if (cmd.command == "KICK") {
		std::cout << "Handling KICK: " << cmd.parameter + cmd.message<< std::endl;
	} else if (cmd.command == "PRIVMSG") {
		std::cout << "Handling PRIVMSG: " << cmd.parameter + cmd.message<< std::endl;
	} else {
		std::cout << "Command not found: " << cmd.command << std::endl;
	}
}

void	Server::createChannel(std::string name)
{
	// // Check if a channel with the given name already exists
	// for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	// {
	// 	if (it->getName() == name)
	// 		return ;
	// }
	// // if does not exist, create a new one and add
	channels.push_back(Channel(name));
}
