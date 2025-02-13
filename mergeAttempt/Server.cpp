/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:34:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/13 16:28:31 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


const char* Server::SeverExceptionSocket::what() const throw()
{
	return "Error: Socket.";
}

const char* Server::SeverExceptionBind::what() const throw()
{
	return "Error: Bind.";
}

const char* Server::SeverExceptionListen::what() const throw()
{
	return "Error: Listen.";
}

Server::Server(): \
	name("default"), password("123456\n"), port(8080)
{
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

int						Server::getPort()const{return (port);}
std::string				Server::getName()const{return (name);}
std::string				Server::getPassword()const{return (password);}
std::vector<Client>		Server::getClients()const{return (clients);}
std::vector<Channel>	Server::getChannels()const{return (channels);}

Server 					&Server::operator=(const Server &src)
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
	return (*this);
}

// void					Server::startServer()
// {
// 	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// 	listen(serverSocket, 5);
// 	std::string from_client = "start";
// 	sockaddr *ptr1 = NULL;
// 	socklen_t *ptr2 = NULL;
// 	while (from_client != "ende")
// 	{
// 		int clientSocket = accept(serverSocket, ptr1, ptr2);
// 		initUser(clientSocket);
// 		std::cout << "New User: " << this->clients.front().getName() << std::endl;
// 	}
// 	close(serverSocket);
// }

std::string					removeNewline(char *buff)
{
	std::string _new = "";

	for (size_t i = 0; i < strlen(buff); i++)
	{
		if (!isalnum(buff[i]))
			break;
		_new += buff[i];
	}
	return _new;
}

void					Server::initUser(int clientSocket)
{
	Client _new;
	std::string from_client;
	char buff[1024] = {0};

	const char *pw_check = "Please enter the password:\n";
	send(clientSocket, pw_check, strlen(pw_check), 0);
	recv(clientSocket, buff, sizeof(buff), 0);
	from_client = removeNewline(buff);
	if (from_client == this->password)
	{
		const char *to_client = "Please enter your user name:\n";
		send(clientSocket, to_client, strlen(to_client), 0);
		recv(clientSocket, buff, sizeof(buff), 0);
		from_client = buff;
		_new.setName(from_client);
		this->clients.push_back(_new);
		{
			const char *to_client = "Thanks for joining :)\nYou may continue now\n";
			send(clientSocket, to_client, strlen(to_client), 0);
		}
		return ;
	}
	const char *to_client = "Wrong Password.\n";
	send(clientSocket, to_client, strlen(to_client), 0);
	close(clientSocket);
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

void Server::launch() {
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		throw SeverExceptionBind();
	}
	std::cout << "Password to join: " << this->password << std::endl;
	// // Print the network address
	// char ipStr[INET_ADDRSTRLEN];
	// inet_ntop(AF_INET, &address.sin_addr, ipStr, INET_ADDRSTRLEN);
	// std::cout << "Server bound to IP: " << ipStr << " on port: " << ntohs(address.sin_port) << std::endl;

	// Mark the socket for listenign in
	if (listen(serverSocket, SOMAXCONN) == -1) {
		throw SeverExceptionListen();
	}
	std::cout << "Server listening on port " << this->port << std::endl;

	//------------------------------------------------//
	//                Poll()                          //
	//------------------------------------------------//

	// to manage multiple clients
	std::vector<struct pollfd> poll_fds;
	pollfd server_poll_fd;
	server_poll_fd.fd = serverSocket;
	server_poll_fd.events = POLLIN;  // constant check for incoming connections
	poll_fds.push_back(server_poll_fd);

	while (true) {
		int activity = poll(&poll_fds[0], poll_fds.size(), -1);  // Wait indefinitely

		if (activity < 0) {
			perror("Poll error");
			continue;
		}

		// Check for new client connections
		if (poll_fds[0].revents & POLLIN) {
			socklen_t addrlen = sizeof(serverAddress);
			int new_socket = accept(serverSocket, (struct sockaddr*)&serverAddress, &addrlen);
			if (new_socket < 0) {
				perror("Accept failed");
				continue;
			}

			std::cout << "New client connected: " << new_socket << std::endl;
			
			initUser(new_socket);
			
			// Add new client to poll list
			pollfd client_fd;
			client_fd.fd = new_socket;
			client_fd.events = POLLIN;  // Monitor for incoming data
			poll_fds.push_back(client_fd);
		}

		// Loop for clients messages
		for (size_t i = 1; i < poll_fds.size(); i++) {
			if (poll_fds[i].revents & POLLIN) {
				int client_fd = poll_fds[i].fd;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);
				int bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);

				if (bytes_read <= 0) {
					std::cout << "Client disconnected: " << client_fd << std::endl;
					close(client_fd);
					poll_fds.erase(poll_fds.begin() + i);
					i--;
				} else {
					std::cout << "Received: " << buffer;
					// send(client_fd, buffer, bytes_read, 0); *** to send it back to client***
				}
			}
		}
	}

	close(serverSocket);
	return ;
}