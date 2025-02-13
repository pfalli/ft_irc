/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehedeman <ehedeman@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:34:33 by ehedeman          #+#    #+#             */
/*   Updated: 2025/02/13 13:59:41 by ehedeman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(): \
	name("default"), password("123456"), port(8080)
{
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(port);
	this->serverAddress.sin_addr.s_addr = INADDR_ANY;
}

Server::Server(std::string _password, int _port, std::string _name): \
	name(_name), password(_password), port(_port)
{
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(port);
	this->serverAddress.sin_addr.s_addr = INADDR_ANY;
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

void					Server::startServer()
{
	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 5);
	std::string from_client = "start";
	sockaddr *ptr1 = NULL;
	socklen_t *ptr2 = NULL;
	while (from_client != "ende")
	{
		int clientSocket = accept(serverSocket, ptr1, ptr2);
		initUser(clientSocket);
		std::cout << "New User: " << this->clients.front().getName() << std::endl;
	}
	close(serverSocket);
}

void					Server::initUser(int clientSocket)
{
	Client _new;
	std::string from_client;
	char buff[1024] = {0};

	const char *to_client = "Please enter your user name.";
	send(clientSocket, to_client, strlen(to_client), 0);
	recv(clientSocket, buff, sizeof(buff), 0);
	from_client = buff;
	_new.setName(from_client);
	this->clients.push_back(_new);
}