/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:34:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/19 13:27:59 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

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

template <typename T>
typename std::vector<T>::iterator		Server::findObject(std::string toFind, std::vector<T> &array)
{
	typename std::vector<T>::iterator it = array.begin();
	while (it != array.end())
	{
		if ((*it).getName() == name)
			break;
		it++;
	}
	return (it);
}


int					Server::initUser(int clientSocket)
{
	Client _new;
	// _new.setUserName(requestName(USERNAME, clientSocket));
	// _new.setNickName(requestName(NICKNAME, clientSocket));
	_new.setSocket(clientSocket);
	this->clients.push_back(_new);
	{
		const char *to_client = "Thanks for joining :)\nYou may continue now\n";
		send(clientSocket, to_client, strlen(to_client), 0);
	}
	return (0);
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
	
	if (initUser(new_socket) == 1)
		return (0);
	// Add new client to poll list
	pollfd client_fd;
	client_fd.fd = new_socket;
	client_fd.events = POLLIN;  // Monitor for incoming data
	poll_fds.push_back(client_fd);
	return (1);
}

int			Server::checkForDisconnect(int client_fd, size_t i, int bytes_read)
{
	if (bytes_read <= 0)
	{
		std::vector<Client>::iterator it = this->clients.begin();
		while (it != this->clients.end())
		{
			if ((*it).getSocket() == client_fd)
				break ;
			it++;
		}
		std::cout	<< "Client disconnected: "	<< (*it).getUserName() + "," <<
		" Socket: "	<< client_fd				<< std::endl;
		this->clients.erase(it);
		close(client_fd);
		poll_fds.erase(poll_fds.begin() + i);
		return (1);
	}
	else
		return (0);
}

bool									Server::sendToNext(std::string buff, int client_fd)
{
	if (this->clients.size() < 2)
		return false;
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

void						Server::getMessages()
{
	for (size_t i = 1; i < poll_fds.size(); i++)
	{
		if (poll_fds[i].revents & POLLIN) {
			int client_fd = poll_fds[i].fd;
			char buffer[BUFFER_SIZE];
			int bytes_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
			std::string str = removeNewline(buffer);
			memset(buffer, 0, BUFFER_SIZE);
			if (checkForDisconnect(client_fd, i, bytes_read))
				i--;
			else
			{
				if (str == "end")
					checkForDisconnect(client_fd, i, -1);
				else if (!sendToNext(str.c_str(), client_fd))
					std::cout << "Received: " << str << std::endl;
				// send(client_fd, buffer, bytes_read, 0); *** to send it back to client***
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
		return ACCEPT_FAILED ;
	}
	return socket;
}

void	Server::launch()
{
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		perror("");
		throw SeverExceptionBind();
	}
	std::cout << "Password to join: " << this->password << std::endl;
	// // Print the network address
	// char ipStr[INET_ADDRSTRLEN];
	// inet_ntop(AF_INET, &address.sin_addr, ipStr, INET_ADDRSTRLEN);
	// std::cout << "Server bound to IP: " << ipStr << " on port: " << ntohs(address.sin_port) << std::endl;

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

	while (true)
	{
		int activity = poll(&poll_fds[0], poll_fds.size(), -1);  // Wait indefinitely

		if (activity < 0)
		{
			perror("Poll error");
			continue;
		}
		if (poll_fds[0].revents & POLLIN)
		{
			// Check for new client connections
			
			
			int new_socket = acceptClient();
			if (new_socket == ACCEPT_FAILED)
				continue ;
			NewClient(new_socket);
		}
		getMessages();
	}
	close(serverSocket);
	return ;
}